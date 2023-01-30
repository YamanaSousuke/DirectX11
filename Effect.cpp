#include "Effect.h"
#include "Vertex.h"

using namespace DirectX;

// ���\�[�X�̏�����
bool Effect::InitAll(ID3D11Device* device)
{
	constantBuffers.assign({ &sceneParameter, &modelParameter, });

	for (auto& constantBuffer : constantBuffers) {
		constantBuffer->CreateBuffer(device);
	}

	return true;
}

// �r���[�s��̐ݒ�
void Effect::SetViewMatrix(const XMVECTOR& eye, const XMVECTOR& focus, const XMVECTOR& up)
{
	auto view = XMMatrixLookAtLH(eye, focus, up);
	auto& sceneParameter = this->sceneParameter;
	XMStoreFloat4x4(&sceneParameter.data.view, XMMatrixTranspose(view));
}

// �v���W�F�N�V�����s��̐ݒ�
void Effect::SetProjectionMatrix(float fov, float aspect, float nearZ, float farZ)
{
	auto projection = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
	auto& sceneParameter = this->sceneParameter;
	XMStoreFloat4x4(&sceneParameter.data.projection, XMMatrixTranspose(projection));
}

// ���[���h�s��̐ݒ�
void Effect::SetWorldMatrix(const XMMATRIX& matrix)
{
	auto& model = modelParameter;
	XMStoreFloat4x4(&model.data.world, XMMatrixTranspose(matrix));
}

// �}�e���A���̐ݒ�
void Effect::SetMaterial(const Material& material)
{
	auto& model = modelParameter;
	model.data.material = material;
}

// �萔�o�b�t�@�[�ƃe�N�X�`�����̓K��
void Effect::Apply(ID3D11DeviceContext* immediateContext)
{
	auto& constantBuffer = constantBuffers;
	constantBuffer[static_cast<int>(Data::Scene)]->BindGS(immediateContext);
	constantBuffer[static_cast<int>(Data::Model)]->BindGS(immediateContext);
	constantBuffer[static_cast<int>(Data::Model)]->BindPS(immediateContext);

	for (auto& buffer : constantBuffers) {
		buffer->UpdateBuffer(immediateContext);
	}
}
