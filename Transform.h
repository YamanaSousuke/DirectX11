#pragma once

#include <DirectXMath.h>

// �g�����X�t�H�[��
class Transform {
public:
	// ���W�̐ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetPosition(float x, float y, float z);

	// ��]�̐ݒ�
	void SetRotation(const DirectX::XMFLOAT3 eulerAngles);

	// ���W�̎擾
	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMVECTOR GetPositionVector() const;

	// ���[���h�ϊ��s��̎擾
	DirectX::XMMATRIX GetWorldMatrix() const;
private:
	// ���W
	DirectX::XMFLOAT3 position = {};
	// ��]
	DirectX::XMFLOAT3 rotation = {};
	// �g��k��
	DirectX::XMFLOAT3 scale = {};
};
