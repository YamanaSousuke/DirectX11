#include "Model.h"

// ���f�����̐ݒ�
void Model::SetModelName(const std::string& name)
{
	modelName = name;
}

// ���f�����̎擾
const char* Model::GetModelName() const
{
	return modelName.c_str();
}

// ���b�V���f�[�^��z��ɒǉ�
void Model::PushMeshData(const Model::MeshData& meshData)
{
	meshList.push_back(meshData);
}

// ���b�V���f�[�^�̎擾
std::vector<Model::MeshData>& Model::GetMeshData()
{
	return meshList;
}