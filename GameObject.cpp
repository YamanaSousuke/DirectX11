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

// �}�e���A���̐ݒ�
void GameObject::SetMaterial(const Material& material)
{
	this->material = material;
}

// �`��
void GameObject::Draw(ID3D11DeviceContext* immediateContext, Effect& effect)
{
	static float time = 0.0f;
	time += 0.01666f;

	// ���_�o�b�t�@�[�ƃC���f�b�N�X�o�b�t�@�[�̐ݒ�
	ID3D11Buffer* vertexBuffers[1] = { vertexBuffer.Get() };
	UINT strides[1] = { vertexStride };
	UINT offsets[1] = { 0 };
	immediateContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, strides, offsets);
	immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	
	effect.SetWorldMatrix(transform.GetWorldMatrix());
	effect.SetMaterial(material);
	effect.Apply(immediateContext);

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
