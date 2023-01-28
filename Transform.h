#pragma once

#include <DirectXMath.h>

// �g�����X�t�H�[��
class Transform {
public:
	// ���W�̐ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetPosition(float x, float y, float z);

	// ���W�̎擾
	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMVECTOR GetPositionVector() const;
private:
	DirectX::XMFLOAT3 position;
};
