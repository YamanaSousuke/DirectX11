#include "Model.h"

// モデル名の設定
void Model::SetModelName(const std::string& name)
{
	modelName = name;
}

// モデル名の取得
const char* Model::GetModelName() const
{
	return modelName.c_str();
}

// メッシュデータを配列に追加
void Model::PushMeshData(const Model::MeshData& meshData)
{
	meshList.push_back(meshData);
}

// メッシュデータの取得
std::vector<Model::MeshData>& Model::GetMeshData()
{
	return meshList;
}