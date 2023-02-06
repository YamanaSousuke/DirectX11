#pragma once

#include <DirectXMath.h>

// �g�����X�t�H�[��
class Transform {
public:
	// ���W�̐ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetPosition(float x, float y, float z);

	// ��]�̐ݒ�
	void SetRotation(const DirectX::XMFLOAT3& eulerAngles);
	void SetRotation(float x, float y, float z);

	// �g��k���̐ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale);
	void SetScale(float x, float y, float z);

	// ���W�̎擾
	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMVECTOR GetPositionVector() const;

	// ��]�̎擾
	DirectX::XMFLOAT3 GetRotation() const;

	// �X�P�[���̎擾
	DirectX::XMFLOAT3 GetScale() const;

	// ���[���h�ϊ��s��̎擾
	DirectX::XMMATRIX GetWorldMatrix() const;
	// ���[���h�ϊ��t�s��̎擾
	DirectX::XMMATRIX GetWorldInverseMatrix() const;
private:
	// ���W
	DirectX::XMFLOAT3 position = {};
	// ��]
	DirectX::XMFLOAT3 rotation = {};
	// �g��k��
	DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
};
