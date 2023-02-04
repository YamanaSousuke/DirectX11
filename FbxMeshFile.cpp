#include "FBXMeshFile.h"
#include "WICTextureLoader.h"

#include <codecvt> 

using namespace DirectX;

// ファイルの読み込み
bool FbxMeshFile::Load(const char* filename, ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	this->device = device;

	if (!GenerateMeshFromFile(filename)) {
		return false;
	}

	if (!CreateVertexBuffer(device, immediateContext)) {
		return false;
	}

	if (!CreateIndexBuffer(device, immediateContext)) {
		return false;
	}

	// UINT byteWidth = sizeof(T);
	// // 16バイトに統一する
	// if (byteWidth % 16 != 0) {
	// 	byteWidth = byteWidth + 16 - byteWidth % 16;
	// }

	D3D11_BUFFER_DESC bufferDesc = { };
	bufferDesc.ByteWidth = sizeof(ModelData);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	const auto hr = device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"定数バッファーの作成に失敗");
		return false;
	}
	return true;
}

// ファイルからメッシュの生成を行う
bool FbxMeshFile::GenerateMeshFromFile(const char* filename)
{
	// FbxManagerの作成
	FbxManager* fbxManager = FbxManager::Create();
	if (fbxManager == nullptr) {
		return false;
	}

	// FbxImporterの作成
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (fbxImporter == nullptr) {
		fbxManager->Destroy();
		return false;
	}

	// FbxSceneの作成
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "");
	if (fbxScene == nullptr) {
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}

	// ファイル名から初期化する
	fbxImporter->Initialize(filename);
	fbxImporter->Import(fbxScene);

	// マテリアル単位でメッシュを分割する
	FbxGeometryConverter converter(fbxManager);
	converter.SplitMeshesPerMaterial(fbxScene, true);
	converter.Triangulate(fbxScene, true);

	// マテリアルの読み込み
	auto materialNum = fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	materialNum = 1;
	printf("material Count : %d\n", materialNum);
	// materialNum = 1;
	for (int i = 0; i < materialNum; i++) {
		LoadMaterial(fbxScene->GetSrcObject<FbxSurfaceMaterial>(i));
	}

	// メッシュ数の取得
	auto numMesh = fbxScene->GetSrcObjectCount<FbxMesh>();
	for (int i = 0; i < numMesh; i++) {
		CreateMesh(fbxScene->GetSrcObject<FbxMesh>(i));
	}

	fbxScene->Destroy();
	fbxImporter->Destroy();
	fbxManager->Destroy();
	return true;
}

// マテリアルデータの読み込み
void FbxMeshFile::LoadMaterial(FbxSurfaceMaterial* material)
{
	Material objectMaterial = {};

	enum class Material {
		Ambient,
		Diffuse,
		Specular,
		Max,
	};

	// 初期化
	FbxDouble3 colors[(int)Material::Max];
	FbxDouble factors[(int)Material::Max];
	FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sAmbient);

	if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
		const char* elementCheckList[] = {
			FbxSurfaceMaterial::sAmbient,
			FbxSurfaceMaterial::sDiffuse,
		};

		const char* factorCheckList[] = {
			FbxSurfaceMaterial::sAmbientFactor,
			FbxSurfaceMaterial::sDiffuseFactor
		};

		// 対応しているマテリアルがあれば設定する
		for (int i = 0; i < 2; i++) {
			property = material->FindProperty(elementCheckList[i]);
			if (property.IsValid()) {
				colors[i] = property.Get<FbxDouble3>();
			}
			else {
				colors[i] = FbxDouble3(1.0, 1.0, 1.0);
			}

			property = material->FindProperty(factorCheckList[i]);
			if (property.IsValid()) {
				factors[i] = property.Get<FbxDouble>();
			}
			else {
				factors[i] = 1.0f;
			}
		}
	}

	// アンビエントカラーの設定
	FbxDouble3 color = colors[(int)Material::Ambient];
	FbxDouble factor = factors[(int)Material::Ambient];
	objectMaterial.ambient = XMFLOAT4((float)color[0], (float)color[1], (float)color[2], (float)factor);

	// ディフューズカラーの設定
	color = colors[(int)Material::Diffuse];
	factor = factors[(int)Material::Diffuse];
	objectMaterial.diffuse = XMFLOAT4((float)color[0], (float)color[1], (float)color[2], (float)factor);
	materials[material->GetName()] = objectMaterial;

	// ディフューズマテリアルからテクスチャー情報の取得
	FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	int numTextures = diffuseProperty.GetSrcObjectCount<FbxFileTexture>();
	FbxFileTexture* texture = nullptr;
	std::string keyword;
	if (numTextures) {
		texture = diffuseProperty.GetSrcObject<FbxFileTexture>();		
	}

	LoadTexture(texture, keyword);

}

// メッシュデータの作成
void FbxMeshFile::CreateMesh(FbxMesh* mesh)
{
	MeshData meshData = {};
	LoadIndices(meshData, mesh);
	LoadVertices(meshData, mesh);
	LoadColors(meshData, mesh);
	LoadNormal(meshData, mesh);
	SetMaterial(meshData, mesh);
	meshList.push_back(meshData);
}


// 頂点インデックスデータを読み込む
void FbxMeshFile::LoadIndices(MeshData& meshData, FbxMesh* mesh)
{
	auto numPolygon = mesh->GetPolygonCount();
	for (int i = 0; i < numPolygon; i++) {
		meshData.indices.push_back(i * 3 + 2);
		meshData.indices.push_back(i * 3 + 1);
		meshData.indices.push_back(i * 3 + 0);
	}
}

// 頂点データを読み込む
void FbxMeshFile::LoadVertices(MeshData& meshData, FbxMesh* mesh)
{
	// 頂点バッファーの取得
	FbxVector4* vertices = mesh->GetControlPoints();
	// インデックスバッファーの作成
	int* indices = mesh->GetPolygonVertices();
	// 頂点座標数の取得
	int vertexCount = mesh->GetPolygonVertexCount();

	for (int i = 0; i < vertexCount; i++) {
		VertexPositionNormalTextureColor vertex = {};
		int index = indices[i];

		// 右手座標系から左手座標系に変換するので-x
		vertex.position.x = (float)-vertices[index][0];
		vertex.position.y = (float)vertices[index][1];
		vertex.position.z = (float)vertices[index][2];
		meshData.vertices.push_back(vertex);
	}
}

// マテリアル名の設定
void FbxMeshFile::SetMaterial(MeshData& meshData, FbxMesh* mesh)
{
	// マテリアルが無ければ終わり
	if (!mesh->GetElementMaterialCount()) {
		meshData.materialName = "";
		return;
	}

	FbxLayerElementMaterial* material = mesh->GetElementMaterial(0);
	int index = material->GetIndexArray().GetAt(0);
	FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

	if (surfaceMaterial) {
		meshData.materialName = surfaceMaterial->GetName();
	}
	else {
		meshData.materialName = "";
	}
}

// 頂点カラーデータを読み込む
void FbxMeshFile::LoadColors(MeshData& meshData, FbxMesh* mesh)
{
	// 頂点カラーデータ数の取得 
	int count = mesh->GetElementVertexColorCount();
	printf("colorCount : %d\n", count);
	if (count == 0) {
		return;
	}

	// 頂点カラーデータの取得
	FbxGeometryElementVertexColor* vertexColors = mesh->GetElementVertexColor();
	if (vertexColors == nullptr) {
		return;
	}

	FbxLayerElement::EMappingMode mappingMode = vertexColors->GetMappingMode();
	FbxLayerElement::EReferenceMode referenceMode = vertexColors->GetReferenceMode();

	// ポリゴンの頂点ごとにマッピングされている場合
	if (mappingMode == FbxLayerElement::eByPolygonVertex && referenceMode == FbxLayerElement::eIndexToDirect) {
		// 要素の取得
		FbxLayerElementArrayTemplate<FbxColor>& colors = vertexColors->GetDirectArray();
		FbxLayerElementArrayTemplate<int>& indices = vertexColors->GetIndexArray();

		// 頂点配列に代入
		for (int i = 0; i < indices.GetCount(); i++) {
			int id = indices.GetAt(i);
			FbxColor color = colors.GetAt(id);
			meshData.vertices[i].color.x = (float)color.mRed;
			meshData.vertices[i].color.y = (float)color.mGreen;
			meshData.vertices[i].color.z = (float)color.mBlue;
			meshData.vertices[i].color.w = (float)color.mAlpha;
		}
	}
}

// 法線データを読み込む
void FbxMeshFile::LoadNormal(MeshData& meshData, FbxMesh* mesh)
{
	FbxArray<FbxVector4> normals = {};
	mesh->GetPolygonVertexNormals(normals);
	for (int i = 0; i < normals.Size(); i++) {
		meshData.vertices[i].normal.x = (float)-normals[i][0];
		meshData.vertices[i].normal.y = (float)normals[i][1];
		meshData.vertices[i].normal.z = (float)normals[i][2];
	}
}

// テクスチャー情報を読み込む
void FbxMeshFile::LoadTexture(FbxFileTexture* textrue, const std::string& keyword)
{
	if (textrue == nullptr) {
		return;
	}

	// ファイル名の取得
	std::string filePath = textrue->GetRelativeFileName();
	printf("filePath : %s\n", filePath.c_str());

	// 「/」または「\\」を後ろから見つける
	auto findSplitPoint = filePath.find_last_of('/');
	if (findSplitPoint == std::string::npos) {
		findSplitPoint = filePath.find_last_of('\\');
	}
	
	// 相対パスからテクスチャの名前だけ取得
	auto textureFileName = filePath.substr(findSplitPoint + 1);
	int filePathLength = static_cast<int>(textureFileName.length());
	printf("textureFileName : %s\n", textureFileName.c_str());

	// ワイド文字に変換して、テクスチャの読み込みを行う
	std::wstring wFileName;
	wFileName.resize(filePathLength);
	MultiByteToWideChar(CP_ACP, 0, textureFileName.c_str(), -1, &wFileName[0], filePathLength);
	const auto hr = DirectX::CreateWICTextureFromFile(device.Get(), wFileName.c_str(), nullptr, texture[textureFileName].GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"テクスチャーの読み込みに失敗\n");
	}
}

// 頂点バッファーの作成
bool FbxMeshFile::CreateVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	for (auto& mesh : meshList) {
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(VertexPositionNormalTextureColor) * (UINT)mesh.vertices.size();
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		const auto hr = device->CreateBuffer(&bufferDesc, nullptr, mesh.vertexBuffer.GetAddressOf());
		if (FAILED(hr)) {
			return false;
			OutputDebugString(L"頂点バッファーの作成に失敗\n");
		}
		immediateContext->UpdateSubresource(mesh.vertexBuffer.Get(), 0, nullptr, mesh.vertices.data(), 0, 0);
	}

	return true;
}

// インデックスバッファーの作成
bool FbxMeshFile::CreateIndexBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	for (auto& mesh : meshList) {
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(UINT) * (UINT)mesh.indices.size();
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		const auto hr = device->CreateBuffer(&bufferDesc, nullptr, mesh.indexBuffer.GetAddressOf());
		if (FAILED(hr)) {
			OutputDebugString(L"インデックスバッファーの作成に失敗\n");
		}
		immediateContext->UpdateSubresource(mesh.indexBuffer.Get(), 0, nullptr, mesh.indices.data(), 0, 0);
	}

	return true;
}

// 描画
void FbxMeshFile::Draw(ID3D11DeviceContext* immediateContext)
{
	static float time = 0.0f;
	time += 0.01666f;
	for (auto& mesh : meshList) {
		ModelData modelData = {};
		XMMATRIX world = XMMatrixIdentity();
		world *= XMMatrixScaling(0.05f, 0.05f, 0.05f);
		// world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(XM_PIDIV2), 0.0f, 0.0f);
		world *= XMMatrixRotationY(-XM_PIDIV2);
		// world *= XMMatrixTranslation(0.0f, 0.0f, 5.0f);

		modelData.world = world;
		modelData.ambient = materials[mesh.materialName].ambient;
		modelData.diffues = materials[mesh.materialName].diffuse;
		modelData.specular = materials[mesh.materialName].specular;

		immediateContext->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &modelData, 0, 0);
		immediateContext->GSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());
		immediateContext->PSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());

		// 頂点バッファーとインデックスバッファーの設定
		ID3D11Buffer* vertexBuffers[1] = { mesh.vertexBuffer.Get() };
		UINT strides[1] = { sizeof(VertexPositionNormalTextureColor) };
		UINT offsets[1] = { 0 };
		immediateContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, strides, offsets);
		immediateContext->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		immediateContext->DrawIndexed((UINT)mesh.indices.size(), 0, 0);
		// immediateContext->Draw((UINT)mesh.vertices.size(), 0);
	}
}
