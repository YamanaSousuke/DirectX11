#include "GameObject.h"

using namespace DirectX;

// �g�����X�t�H�[���̎擾
Transform& GameObject::GetTransform()
{
	return transform;
}

// �e�N�X�`���̐ݒ�
void GameObject::SetTexture(ID3D11ShaderResourceView* texture)
{
	this->texture = texture;
}

// ���W�̐ݒ�
void GameObject::SetPosition(const XMFLOAT3& position)
{
	this->position = position;
}

// �`��
void GameObject::Draw(ID3D11DeviceContext* immediateContext)
{
	// ���_�o�b�t�@�[�ƃC���f�b�N�X�o�b�t�@�[�̐ݒ�
	ID3D11Buffer* vertexBuffers[1] = { vertexBuffer.Get()};
	UINT strides[1] = { vertexStride };
	UINT offsets[1] = { 0 };
	immediateContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, strides, offsets);
	immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	ModelParameter modelParameter = {};
	static float time = 0.0f;
	time += 0.01666f;

	// ���[���h�s��
	XMMATRIX world = XMMatrixIdentity();
	XMVECTOR axis = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);
	world *= XMMatrixRotationAxis(axis, time * 0.5f);

	auto position = transform.GetPosition();
	world *= XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&modelParameter.world, XMMatrixTranspose(world));

	// �萔�o�b�t�@�[���擾���āA�f�[�^�̓]�����s��
	ComPtr<ID3D11Buffer> constnatBuffer = nullptr;
	immediateContext->GSGetConstantBuffers(1, 1, constnatBuffer.GetAddressOf());
	immediateContext->UpdateSubresource(constnatBuffer.Get(), 0, nullptr, &modelParameter, 0, 0);

	// �e�N�X�`���̐ݒ�
	ID3D11ShaderResourceView* textureViews[1] = { texture.Get() };
	immediateContext->PSSetShaderResources(0, 1, textureViews);
	immediateContext->DrawIndexed(indexCount, 0, 0);
}

// ���\�[�X�̉��
void GameObject::Release()
{
	delete this;
}
