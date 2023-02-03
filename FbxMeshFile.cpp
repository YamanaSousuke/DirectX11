#include "FBXMeshFile.h"

using namespace DirectX;

// �t�@�C���̓ǂݍ���
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

	// UINT byteWidth = sizeof(T);
	// // 16�o�C�g�ɓ��ꂷ��
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
		OutputDebugString(L"�萔�o�b�t�@�[�̍쐬�Ɏ��s");
		return false;
	}
	return true;
}

// �t�@�C�����烁�b�V���̐������s��
bool FbxMeshFile::GenerateMeshFromFile(const char* filename)
{
	// FbxManager�̍쐬
	FbxManager* fbxManager = FbxManager::Create();
	if (fbxManager == nullptr) {
		return false;
	}

	// FbxImporter�̍쐬
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (fbxImporter == nullptr) {
		fbxManager->Destroy();
		return false;
	}

	// FbxScene�̍쐬
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "");
	if (fbxScene == nullptr) {
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}

	// �t�@�C�������珉��������
	fbxImporter->Initialize(filename);
	fbxImporter->Import(fbxScene);

	// �}�e���A���P�ʂŃ��b�V���𕪊�����
	FbxGeometryConverter converter(fbxManager);
	converter.SplitMeshesPerMaterial(fbxScene, true);
	converter.Triangulate(fbxScene, true);

	// �}�e���A���̓ǂݍ���
	auto materialNum = fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	printf("material Count : %d\n", materialNum);
	for (int i = 0; i < materialNum; i++) {
		LoadMaterial(fbxScene->GetSrcObject<FbxSurfaceMaterial>(i));
	}

	// ���b�V�����̎擾
	auto numMesh = fbxScene->GetSrcObjectCount<FbxMesh>();
	for (int i = 0; i < numMesh; i++) {
		CreateMesh(fbxScene->GetSrcObject<FbxMesh>(i));
	}

	fbxScene->Destroy();
	fbxImporter->Destroy();
	fbxManager->Destroy();
	return true;
}

// �}�e���A���f�[�^�̓ǂݍ���
void FbxMeshFile::LoadMaterial(FbxSurfaceMaterial* material)
{
	Material objectMaterial = {};

	enum class Material {
		Ambient,
		Diffuse,
		Specular,
		Max,
	};

	// ������
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

		// �Ή����Ă���}�e���A��������ΐݒ肷��
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

	// �A���r�G���g�J���[�̐ݒ�
	FbxDouble3 color = colors[(int)Material::Ambient];
	FbxDouble factor = factors[(int)Material::Ambient];
	printf("ambient r : %lf, g : %lf, b : %lf\n", (float)color[0], (float)color[1], (float)color[2]);
	objectMaterial.ambient = XMFLOAT4((float)color[0], (float)color[1], (float)color[2], (float)factor);

	// �f�B�t���[�Y�J���[�̐ݒ�
	color = colors[(int)Material::Diffuse];
	factor = factors[(int)Material::Diffuse];
	printf("diffuse r : %lf, g : %lf, b : %lf\n", (float)color[0], (float)color[1], (float)color[2]);
	objectMaterial.diffuse = XMFLOAT4((float)color[0], (float)color[1], (float)color[2], (float)factor);
	materials[material->GetName()] = objectMaterial;
}

// ���b�V���f�[�^�̍쐬
void FbxMeshFile::CreateMesh(FbxMesh* mesh)
{
	MeshData meshData = {};
	LoadIndices(meshData, mesh);
	LoadVertices(meshData, mesh);
	LoadColors(meshData, mesh);
	SetMaterial(meshData, mesh);
	meshList.push_back(meshData);
}


// ���_�C���f�b�N�X�f�[�^��ǂݍ���
void FbxMeshFile::LoadIndices(MeshData& meshData, FbxMesh* mesh)
{
	auto numPolygon = mesh->GetPolygonCount();
	for (int i = 0; i < numPolygon; i++) {
		meshData.indices.push_back(i * 3 + 2);
		meshData.indices.push_back(i * 3 + 1);
		meshData.indices.push_back(i * 3 + 0);
	}
}

// ���_�f�[�^��ǂݍ���
void FbxMeshFile::LoadVertices(MeshData& meshData, FbxMesh* mesh)
{
	// ���_�o�b�t�@�[�̎擾
	FbxVector4* vertices = mesh->GetControlPoints();
	// �C���f�b�N�X�o�b�t�@�[�̍쐬
	int* indices = mesh->GetPolygonVertices();
	// ���_���W���̎擾
	UINT vertexCount = mesh->GetPolygonVertexCount();

	for (int i = 0; i < vertexCount; i++) {
		VertexPosition vertexPosition = {};
		int index = indices[i];

		// �E����W�n���獶����W�n�ɕϊ�����̂�-x
		vertexPosition.position.x = (float)-vertices[index][0];
		vertexPosition.position.y = (float)vertices[index][1];
		vertexPosition.position.z = (float)vertices[index][2];
		meshData.vertices.push_back(vertexPosition);
	}
}

// �}�e���A�����̐ݒ�
void FbxMeshFile::SetMaterial(MeshData& meshData, FbxMesh* mesh)
{
	// �}�e���A����������ΏI���
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

// ���_�J���[�f�[�^��ǂݍ���
void FbxMeshFile::LoadColors(MeshData& meshData, FbxMesh* mesh)
{
	// ���_�J���[�f�[�^���̎擾 
	int count = mesh->GetElementVertexColorCount();
	if (count == 0) {
		return;
	}

	// ���_�J���[�f�[�^�̎擾 
	FbxGeometryElementVertexColor* vertexColors = mesh->GetElementVertexColor();
	if (vertexColors == nullptr) {
		return;
	}

	FbxLayerElement::EMappingMode mappingMode = vertexColors->GetMappingMode();
	FbxLayerElement::EReferenceMode referenceMode = vertexColors->GetReferenceMode();
	if (mappingMode == FbxLayerElement::eByPolygonVertex && referenceMode == FbxLayerElement::eIndexToDirect) {
		FbxLayerElementArrayTemplate<FbxColor>& colors = vertexColors->GetDirectArray();
		FbxLayerElementArrayTemplate<int>& indices = vertexColors->GetIndexArray();

		for (int i = 0; i < indices.GetCount(); i++) {
			int id = indices.GetAt(i);
			FbxColor color = colors.GetAt(id);

		}
	}

}

// ���_�o�b�t�@�[�̍쐬
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
			OutputDebugString(L"���_�o�b�t�@�[�̍쐬�Ɏ��s\n");
		}
		immediateContext->UpdateSubresource(mesh.vertexBuffer.Get(), 0, nullptr, mesh.vertices.data(), 0, 0);
	}

	return true;
}

// �C���f�b�N�X�o�b�t�@�[�̍쐬
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
			OutputDebugString(L"�C���f�b�N�X�o�b�t�@�[�̍쐬�Ɏ��s\n");
		}
		immediateContext->UpdateSubresource(mesh.indexBuffer.Get(), 0, nullptr, mesh.indices.data(), 0, 0);
	}

	return true;
}

// �`��
void FbxMeshFile::Draw(ID3D11DeviceContext* immediateContext)
{
	static float time = 0.0f;
	time += 0.01666f;
	for (auto& mesh : meshList) {
		ModelData modelData = {};
		XMMATRIX world = XMMatrixIdentity();
		world *= XMMatrixRotationRollPitchYaw(time, time, time);
		modelData.world = world;
		modelData.ambient = materials[mesh.materialName].ambient;
		modelData.diffues = materials[mesh.materialName].diffuse;
		modelData.specular = materials[mesh.materialName].specular;

		immediateContext->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &modelData, 0, 0);
		immediateContext->GSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());
		immediateContext->PSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());

		// ���_�o�b�t�@�[�ƃC���f�b�N�X�o�b�t�@�[�̐ݒ�
		ID3D11Buffer* vertexBuffers[1] = { mesh.vertexBuffer.Get() };
		UINT strides[1] = { sizeof(VertexPosition) };
		UINT offsets[1] = { 0 };
		immediateContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, strides, offsets);
		immediateContext->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		immediateContext->DrawIndexed((UINT)mesh.indices.size(), 0, 0);
	}
}
