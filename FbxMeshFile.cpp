#include "FBXMeshFile.h"

using namespace DirectX;

// ファイルの読み込み
bool FbxMeshFile::Load(const char* filename, ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	if (!GenerateMeshFromFile(filename)) {
		return false;
	}

	if (!CreateVertexBuffer(device, immediateContext)) {
		return false;
	}

	if (!CreateIndexBuffer(device, immediateContext)) {
		return false;
	}

	// if (!CreatrInputLayout(device)) {
	// 	return false;
	// }

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

	printf("numMesh : %zu\n", meshList.size());
	int loopCount = 0;

	for (int i = 0; i < meshList.size(); i++) {
		for (int j = 0; j < meshList[i].vertices.size(); j++) {
			printf("x : %.2lf, ", (float)meshList[i].vertices[j].position.x);
			printf("y : %.2lf, ", (float)meshList[i].vertices[j].position.y);
			printf("z : %.2lf\n", (float)meshList[i].vertices[j].position.z);
			loopCount++;
		}
	}
	
	printf("--------------loopCount------------------ : %d\n", loopCount);
	for (int i = 0; i < meshList.size(); i++) {
		for (int j = 0; j < meshList[i].indices.size(); j++) {
			printf("indices : %d : ", meshList[i].indices[j]);
			loopCount++;
		}
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
	// FbxGeometryConverter converter(fbxManager);
	// converter.SplitMeshesPerMaterial(fbxScene, true);
	// converter.Triangulate(fbxScene, true);

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
	enum class Material {
		Ambient,
		Diffuse,
		Specular,
		Max,
	};

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

	FbxDouble3 ambientColor = colors[(int)Material::Ambient];
	FbxDouble ambientFactor = factors[(int)Material::Ambient];


}

// メッシュデータの作成
void FbxMeshFile::CreateMesh(FbxMesh* mesh)
{
	MeshData meshData = {};
	LoadIndices(meshData, mesh);
	LoadVertices(meshData, mesh);
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
	UINT vertexCount = mesh->GetPolygonVertexCount();

	for (int i = 0; i < vertexCount; i++) {
		VertexPosition vertexPosition = {};
		int index = indices[i];

		// 右手座標系から左手座標系に変換するので-x
		vertexPosition.position.x = (float)-vertices[index][0];
		vertexPosition.position.y = (float)vertices[index][1];
		vertexPosition.position.z = (float)vertices[index][2];
		meshData.vertices.push_back(vertexPosition);
	}
}

// 頂点バッファーの作成
bool FbxMeshFile::CreateVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	for (auto& mesh : meshList) {
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(VertexPosition) * (UINT)mesh.vertices.size();
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
		bufferDesc.ByteWidth = sizeof(UINT) * mesh.indices.size();
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

// インプットレイアウトの作成
bool FbxMeshFile::CreatrInputLayout(ID3D11Device* device)
{
	const auto hr = device->CreateInputLayout(VertexPosition::inputLayout, ARRAYSIZE(VertexPosition::inputLayout),
		g_VertexShader, ARRAYSIZE(g_VertexShader), inputLayout.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"インプットレイアウトの作成に失敗\n");
		return false;
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
		world *= XMMatrixRotationRollPitchYaw(time, time, time);
		modelData.world = world;
		immediateContext->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &modelData, 0, 0);
		immediateContext->GSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());
		// 頂点バッファーとインデックスバッファーの設定
		ID3D11Buffer* vertexBuffers[1] = { mesh.vertexBuffer.Get() };
		UINT strides[1] = { sizeof(VertexPosition) };
		UINT offsets[1] = { 0 };
		immediateContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, strides, offsets);
		immediateContext->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		immediateContext->DrawIndexed((UINT)mesh.indices.size(), 0, 0);
	}
}
