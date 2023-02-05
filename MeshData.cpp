#include "MeshData.h"

// テクスチャーの名前の取得
std::string MeshData::GetTextureName() const
{
	return textureName;
}

// テクスチャーが有効かどうか
bool MeshData::IsValidTexture() const
{
	return isValidTexture;
}

// テクスチャー名の設定
void MeshData::SetTextureName(const std::string& name)
{
	isValidTexture = true;
	textureName = name;
}
