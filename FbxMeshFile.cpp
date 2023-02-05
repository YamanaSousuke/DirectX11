#include "FBXMeshFile.h"
#include "WICTextureLoader.h"

using namespace DirectX;

// 初期化
void FbxMeshFile::Init(ID3D11Device* device)
{
	this->device = device;
}

// ファイルの読み込み
FbxMeshFile* FbxMeshFile::Load(const std::string& filename, ID3D11DeviceContext* immediateContext)
{
	// 最後の「/」または「\\」で文字列を分割する
	int filePathLength = static_cast<int>(filename.length());
	auto findSplitPoint = filename.find_last_of('/');
	fileNameBeforeSplit = filename.substr(0, findSplitPoint + 1);
	fileNameAfterSpilt = filename.substr(findSplitPoint + 1);

	if (!GenerateMeshFromFile(filename)) {
		return nullptr;
	}

	if (!CreateVertexBuffer(immediateContext)) {
		return nullptr;
	}

	if (!CreateIndexBuffer(immediateContext)) {
		return nullptr;
	}

	// D3D11_BUFFER_DESC bufferDesc = { };
	// bufferDesc.ByteWidth = sizeof(ModelData);
	// bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// bufferDesc.CPUAccessFlags = 0;
	// bufferDesc.MiscFlags = 0;
	// bufferDesc.StructureByteStride = 0;
	// const auto hr = device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
	// if (FAILED(hr)) {
	// 	OutputDebugString(L"定数バッファーの作成に失敗");
	// 	return nullptr;
	// }
	return this;
}

// ファイルからメッシュの生成を行う
bool FbxMeshFile::GenerateMeshFromFile(const std::string& filename)
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
	fbxImporter->Initialize(filename.c_str());
	fbxImporter->Import(fbxScene);

	// マテリアル単位でメッシュを分割する
	FbxGeometryConverter converter(fbxManager);
	converter.SplitMeshesPerMaterial(fbxScene, true);
	converter.Triangulate(fbxScene, true);

	// メッシュ数の取得
	auto numMesh = fbxScene->GetSrcObjectCount<FbxMesh>();
	for (int i = 0; i < numMesh; i++) {
		CreateMesh(fbxScene->GetSrcObject<FbxMesh>(i));
	}

	// マテリアルの読み込み
	materialCount =static_cast<UINT>(fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>());
	for (UINT i = 0; i < materialCount; i++) {
		LoadMaterial(fbxScene->GetSrcObject<FbxSurfaceMaterial>(i));
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
	FbxDouble3 colors[(int)Material::Max] = {};
	FbxDouble factors[(int)Material::Max] = {};
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
	else
	{
		printf("FbxSurfacePhong\n");
	}

	// アンビエントカラーの設定
	FbxDouble3 color = colors[(int)Material::Ambient];
	FbxDouble factor = factors[(int)Material::Ambient];
	objectMaterial.ambient = XMFLOAT4((float)color[0], (float)color[1], (float)color[2], (float)factor);
	printf("ambient x : %lf y : %lf z : %lf factor : %lf\n", (float)color[0], (float)color[1], (float)color[2], (float)factor);

	// ディフューズカラーの設定
	color = colors[(int)Material::Diffuse];
	factor = factors[(int)Material::Diffuse];
	objectMaterial.diffuse = XMFLOAT4((float)color[0], (float)color[1], (float)color[2], (float)factor);
	printf("diffuse x : %lf y : %lf z : %lf factor : %lf\n", (float)color[0], (float)color[1], (float)color[2], (float)factor);

	// マテリアルの登録
	// auto materialName = material->GetName();
	materials[material->GetName()] = objectMaterial;
	printf("materialName %s\n", material->GetName());

	// ディフューズマテリアルからテクスチャー情報の取得
	FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	int numTextures = diffuseProperty.GetSrcObjectCount<FbxFileTexture>();
	FbxFileTexture* texture = nullptr;
	std::string keyword;
	if (numTextures) {
		texture = diffuseProperty.GetSrcObject<FbxFileTexture>();		
	}

	// マテリアルとテクスチャーを関連付ける
	if (LoadTexture(texture, keyword)) {
		materialLinks[material->GetName()] = this->texture[keyword];
	}
}

// メッシュデータの作成
void FbxMeshFile::CreateMesh(FbxMesh* mesh)
{
	MeshData meshData = {};
	LoadIndices(meshData, mesh);
    LoadVertices(meshData, mesh);
	LoadColors(meshData, mesh);
	LoadNormal(meshData, mesh);
	LoadUV(meshData, mesh);
	SetMaterial(meshData, mesh);
	meshList.push_back(meshData);
}

// 頂点インデックスデータを読み込む
void FbxMeshFile::LoadIndices(MeshData& meshData, FbxMesh* mesh)
{
	auto numPolygon = mesh->GetPolygonCount();
	const int triangle = 3;
	for (int i = 0; i < numPolygon; i++) {
		meshData.indices.push_back(i * triangle + 2);
		meshData.indices.push_back(i * triangle + 1);
		meshData.indices.push_back(i * triangle);
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
bool FbxMeshFile::LoadTexture(FbxFileTexture* textrue, std::string& keyword)
{
	if (textrue == nullptr) {
		return false;
	}

	// ファイル名の取得
	std::string filePath = textrue->GetRelativeFileName();
	// 「/」または「\\」を後ろから見つける
	auto findSplitPoint = filePath.find_last_of('/');
	if (findSplitPoint == std::string::npos) {
		findSplitPoint = filePath.find_last_of('\\');
	}
	
	// 相対パスからテクスチャの名前だけ取得
	auto textureFileName = filePath.substr(findSplitPoint + 1);
	auto test = fileNameBeforeSplit + textureFileName;
	int filePathLength = static_cast<int>(test.length());

	// ワイド文字に変換して、テクスチャの読み込みを行う
	std::wstring wFileName;
	wFileName.resize(filePathLength);
	MultiByteToWideChar(CP_ACP, 0, test.c_str(), -1, &wFileName[0], filePathLength);
	const auto hr = DirectX::CreateWICTextureFromFile(device.Get(), wFileName.c_str(), nullptr, texture[textureFileName].GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"テクスチャーの読み込みに失敗\n");
		return false;
	}

	textureNamess.emplace_back(textureFileName);
	keyword = textureFileName;
	return true;
}

// UV座標の読み込み
void FbxMeshFile::LoadUV(MeshData& meshData, FbxMesh* mesh)
{
	FbxStringList uvNames;
	mesh->GetUVSetNames(uvNames);

	FbxArray<FbxVector2> uvBuffer;
	mesh->GetPolygonVertexUVs(uvNames.GetStringAt(0), uvBuffer);
	for (int i = 0; i < uvBuffer.Size(); i++) {
		FbxVector2& uv = uvBuffer[i];
		meshData.vertices[i].texCoord.x = (float)uv[0];
		meshData.vertices[i].texCoord.y = (float)(1.0f - uv[1]);
	}
}

// 頂点バッファーの作成
bool FbxMeshFile::CreateVertexBuffer(ID3D11DeviceContext* immediateContext)
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
			OutputDebugString(L"頂点バッファーの作成に失敗\n");
			return false;
		}
		immediateContext->UpdateSubresource(mesh.vertexBuffer.Get(), 0, nullptr, mesh.vertices.data(), 0, 0);
	}

	return true;
}

// インデックスバッファーの作成
bool FbxMeshFile::CreateIndexBuffer(ID3D11DeviceContext* immediateContext)
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
		world *= XMMatrixRotationY(-XM_PIDIV2 * time * 0.1f);
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


		if (materialLinks.count(mesh.materialName)) {
			immediateContext->PSSetShaderResources(0, 1, materialLinks[mesh.materialName].GetAddressOf());
		}
		else {
			immediateContext->PSSetShaderResources(0, 0, nullptr);
		}
		immediateContext->DrawIndexed((UINT)mesh.indices.size(), 0, 0);
		// immediateContext->Draw((UINT)mesh.vertices.size(), 0);
	}
}

// メッシュ数の取得
size_t FbxMeshFile::GetMeshCount() const 
{
	return meshList.size();
}

// メッシュデータの取得
MeshData FbxMeshFile::GetMeshData(int index) const
{
	return meshList.at(index);
}

// シェーダーリソースビューの取得
ID3D11ShaderResourceView* FbxMeshFile::GetShaderResourceView(int index) const
{
	std::string key = meshList.at(index).materialName;
	if (materialLinks.count(key)) {
		return materialLinks.at(key).Get();
	}
	else {
		return nullptr;
	}
}

// マテリアルの取得
Material FbxMeshFile::GetMaterial(int index) const
{
	std::string key = meshList.at(index).materialName;
	return materials.at(key);
}

// パラメーターの取得
std::string FbxMeshFile::GetFbxFileName() const
{
	return fileNameAfterSpilt;
}

std::vector<std::string> FbxMeshFile::GetTextureName() const
{
	std::vector<std::string> ret;
	for (auto i = texture.begin(); i != texture.end(); i++) {
		ret.emplace_back(i->first);
	}
	return ret;
}

std::string FbxMeshFile::GetTextureName(int index) const
{
	return textureNamess.at(index);
}

UINT FbxMeshFile::GetMaterialCount() const
{
	return materialCount;
}

UINT FbxMeshFile::GetTextureCount() const
{
	return static_cast<UINT>(texture.size());
}