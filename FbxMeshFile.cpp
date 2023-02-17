#include "FBXMeshFile.h"
#include "WICTextureLoader.h"

using namespace DirectX;

// ������
void FbxMeshFile::Init(ID3D11Device* device)
{
	this->device = device;
}

// �t�@�C���̓ǂݍ���
Model FbxMeshFile::Load(const std::string& filename, ID3D11DeviceContext* immediateContext)
{
	// �Ō�́u/�v�܂��́u\\�v�ŕ�����𕪊�����
	int filePathLength = static_cast<int>(filename.length());
	auto findSplitPoint = filename.find_last_of('/');
	fileNameBeforeSplit = filename.substr(0, findSplitPoint + 1);
	model.SetModelName(filename.substr(findSplitPoint + 1));

	GenerateMeshFromFile(filename);

	// �o�b�t�@�[�̍쐬
	CreateBuffer(immediateContext);

	return model;
}

// �t�@�C�����烁�b�V���̐������s��
bool FbxMeshFile::GenerateMeshFromFile(const std::string& filename)
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
	fbxImporter->Initialize(filename.c_str());
	fbxImporter->Import(fbxScene);

	// �}�e���A���P�ʂŃ��b�V���𕪊�����
	FbxGeometryConverter converter(fbxManager);
	converter.SplitMeshesPerMaterial(fbxScene, true);
	converter.Triangulate(fbxScene, true);

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
void FbxMeshFile::LoadMaterial(Model::MeshData& meshData, FbxMesh* mesh)
{
	enum class Material {
		Ambient,
		Diffuse,
		Specular,
		Max,
	};

	// ������
	FbxDouble3 colors[(int)Material::Max] = {};
	FbxDouble factors[(int)Material::Max] = {};

	// ���b�V������}�e���A���̎擾
	FbxLayerElementMaterial* elementMaterial = mesh->GetElementMaterial(0);
	int index = elementMaterial->GetIndexArray().GetAt(0);
	FbxSurfaceMaterial* material = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);
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
	else
	{
		printf("FbxSurfacePhong\n");
	}

	// �A���r�G���g�J���[�̐ݒ�
	FbxDouble3 color = colors[(int)Material::Ambient];
	FbxDouble factor = factors[(int)Material::Ambient];
	meshData.material.ambient = XMFLOAT4((float)color[0], (float)color[1], (float)color[2], (float)factor);

	// �f�B�t���[�Y�J���[�̐ݒ�
	color = colors[(int)Material::Diffuse];
	factor = factors[(int)Material::Diffuse];
	meshData.material.diffuse = XMFLOAT4((float)color[0], (float)color[1], (float)color[2], (float)factor);

	// �f�B�t���[�Y�}�e���A������e�N�X�`���[���̎擾
	FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	int numTextures = diffuseProperty.GetSrcObjectCount<FbxFileTexture>();
	FbxFileTexture* texture = nullptr;
	if (numTextures) {
		texture = diffuseProperty.GetSrcObject<FbxFileTexture>();		
	}

	if (texture != nullptr) {
		//�u/�v�܂��́u\\�v����납�猩����
		std::string filePath = texture->GetRelativeFileName();
		auto findSplitPoint = filePath.find_last_of('/');
		if (findSplitPoint == std::string::npos) {
			findSplitPoint = filePath.find_last_of('\\');
		}

		// ���΃p�X����e�N�X�`���̖��O�����擾
		auto textureFileName = filePath.substr(findSplitPoint + 1);
		auto test = fileNameBeforeSplit + textureFileName;
		int filePathLength = static_cast<int>(test.length());

		// ���C�h�����ɕϊ����āA�e�N�X�`���̓ǂݍ��݂��s��
		std::wstring wFileName;
		wFileName.resize(filePathLength);
		MultiByteToWideChar(CP_ACP, 0, test.c_str(), -1, &wFileName[0], filePathLength);
		const auto hr = DirectX::CreateWICTextureFromFile(device.Get(), wFileName.c_str(), nullptr, meshData.texture.GetAddressOf());
		if (FAILED(hr)) {
			OutputDebugString(L"�e�N�X�`���[�̓ǂݍ��݂Ɏ��s\n");
		}

		// �e�N�X�`���[����o�^����
		meshData.textureName = textureFileName;
	}
}

// ���b�V���f�[�^�̍쐬
void FbxMeshFile::CreateMesh(FbxMesh* mesh)
{
	Model::MeshData meshData = {};
	LoadIndices(meshData, mesh);
    LoadVertices(meshData, mesh);
	LoadColors(meshData, mesh);
	LoadNormal(meshData, mesh);
	LoadUV(meshData, mesh);
	SetMaterial(meshData, mesh);
	LoadMaterial(meshData, mesh);
	model.PushMeshData(meshData);
}

// ���_�C���f�b�N�X�f�[�^��ǂݍ���
void FbxMeshFile::LoadIndices(Model::MeshData& meshData, FbxMesh* mesh)
{
	auto numPolygon = mesh->GetPolygonCount();
	const int triangle = 3;
	for (int i = 0; i < numPolygon; i++) {
		meshData.indices.push_back(i * triangle + 2);
		meshData.indices.push_back(i * triangle + 1);
		meshData.indices.push_back(i * triangle);
	}
}

// ���_�f�[�^��ǂݍ���
void FbxMeshFile::LoadVertices(Model::MeshData& meshData, FbxMesh* mesh)
{
	// ���_�o�b�t�@�[�̎擾
	FbxVector4* vertices = mesh->GetControlPoints();
	// �C���f�b�N�X�o�b�t�@�[�̍쐬
	int* indices = mesh->GetPolygonVertices();
	// ���_���W���̎擾
	int vertexCount = mesh->GetPolygonVertexCount();

	for (int i = 0; i < vertexCount; i++) {
		VertexPositionNormalTextureColor vertex = {};
		int index = indices[i];

		// �E����W�n���獶����W�n�ɕϊ�����̂�-x
		vertex.position.x = (float)-vertices[index][0];
		vertex.position.y = (float)vertices[index][1];
		vertex.position.z = (float)vertices[index][2];
		meshData.vertices.push_back(vertex);
	}
}

// �}�e���A�����̐ݒ�
void FbxMeshFile::SetMaterial(Model::MeshData& meshData, FbxMesh* mesh)
{
	if (mesh->GetElementMaterialCount()) {
		FbxLayerElementMaterial* material = mesh->GetElementMaterial(0);
		int index = material->GetIndexArray().GetAt(0);
		FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

		if (surfaceMaterial) {
			meshData.materialName = surfaceMaterial->GetName();
		}
	}
}

// ���_�J���[�f�[�^��ǂݍ���
void FbxMeshFile::LoadColors(Model::MeshData& meshData, FbxMesh* mesh)
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

	// �|���S���̒��_���ƂɃ}�b�s���O����Ă���ꍇ
	if (mappingMode == FbxLayerElement::eByPolygonVertex && referenceMode == FbxLayerElement::eIndexToDirect) {
		// �v�f�̎擾
		FbxLayerElementArrayTemplate<FbxColor>& colors = vertexColors->GetDirectArray();
		FbxLayerElementArrayTemplate<int>& indices = vertexColors->GetIndexArray();

		// ���_�z��ɑ��
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

// �@���f�[�^��ǂݍ���
void FbxMeshFile::LoadNormal(Model::MeshData& meshData, FbxMesh* mesh)
{
	FbxArray<FbxVector4> normals = {};
	mesh->GetPolygonVertexNormals(normals);
	for (int i = 0; i < normals.Size(); i++) {
		meshData.vertices[i].normal.x = (float)-normals[i][0];
		meshData.vertices[i].normal.y = (float)normals[i][1];
		meshData.vertices[i].normal.z = (float)normals[i][2];
	}
}

// UV���W�̓ǂݍ���
void FbxMeshFile::LoadUV(Model::MeshData& meshData, FbxMesh* mesh)
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

// ���_�o�b�t�@�[�A�C���f�b�N�X�o�b�t�@�[�̍쐬
void FbxMeshFile::CreateBuffer(ID3D11DeviceContext* immediateContext)
{
	auto& meshList = model.GetMeshData();
	for (auto& mesh : meshList) {
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(VertexPositionNormalTextureColor) * (UINT)mesh.vertices.size();
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		auto hr = device->CreateBuffer(&bufferDesc, nullptr, mesh.vertexBuffer.GetAddressOf());
		if (FAILED(hr)) {
			OutputDebugString(L"���_�o�b�t�@�[�̍쐬�Ɏ��s\n");
		}
		immediateContext->UpdateSubresource(mesh.vertexBuffer.Get(), 0, nullptr, mesh.vertices.data(), 0, 0);

		bufferDesc.ByteWidth = sizeof(UINT) * (UINT)mesh.indices.size();
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		hr = device->CreateBuffer(&bufferDesc, nullptr, mesh.indexBuffer.GetAddressOf());
		if (FAILED(hr)) {
			OutputDebugString(L"�C���f�b�N�X�o�b�t�@�[�̍쐬�Ɏ��s\n");
		}
		immediateContext->UpdateSubresource(mesh.indexBuffer.Get(), 0, nullptr, mesh.indices.data(), 0, 0);
	}
}