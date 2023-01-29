#include "GameObject.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

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
void GameObject::Draw(ID3D11DeviceContext* immediateContext)
{
	ImGui::Begin("Debug");
	ImGui::SliderFloat("Smoothness", &material.smooth, 0.0f, 1.0f);
	ImGui::SliderFloat("Metallic", &material.metallic, 0.0f, 1.0f);
	ImGui::End();

	static float time = 0.0f;
	time += 0.01666f;

	// ���_�o�b�t�@�[�ƃC���f�b�N�X�o�b�t�@�[�̐ݒ�
	ID3D11Buffer* vertexBuffers[1] = { vertexBuffer.Get() };
	UINT strides[1] = { vertexStride };
	UINT offsets[1] = { 0 };
	immediateContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, strides, offsets);
	immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// ���[���h�s��
	// XMMATRIX world = transform.GetWorldMatrix();
	ModelParameter modelParameter = {};
    // XMStoreFloat4x4(&modelParameter.world, XMMatrixTranspose(world));
	
	effect.SetWorldMatrix(transform.GetWorldMatrix());
	effect.SetMaterial(material);


	// modelParameter.material = material;


	// �萔�o�b�t�@�[���擾���āA�f�[�^�̓]�����s��
	ComPtr<ID3D11Buffer> constnatBuffer = nullptr;
	immediateContext->GSGetConstantBuffers(1, 1, constnatBuffer.GetAddressOf());
	// effect.UpdateModelParameter(immediateContext);
	immediateContext->UpdateSubresource(constnatBuffer.Get(), 0, nullptr, &modelParameter, 0, 0);

	// �e�N�X�`���̐ݒ�
	ID3D11ShaderResourceView* textureViews[1] = { texture.Get() };
	immediateContext->PSSetShaderResources(0, 1, textureViews);
	immediateContext->DrawIndexed(indexCount, 0, 0);
}

// ���f�����̃T�C�Y�̎擾
size_t GameObject::GetModelParameterSize() const
{
	return sizeof(ModelParameter);
}

// ���\�[�X�̉��
void GameObject::Release()
{
	delete this;
}
