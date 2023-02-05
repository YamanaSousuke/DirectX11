#include "MeshData.h"

// �e�N�X�`���[�̖��O�̎擾
std::string MeshData::GetTextureName() const
{
	return textureName;
}

// �e�N�X�`���[���L�����ǂ���
bool MeshData::IsValidTexture() const
{
	return isValidTexture;
}

// �e�N�X�`���[���̐ݒ�
void MeshData::SetTextureName(const std::string& name)
{
	isValidTexture = true;
	textureName = name;
}
