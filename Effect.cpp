#include "Effect.h"
#include "Vertex.h"

#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

using namespace DirectX;

// ���\�[�X�̏�����
bool Effect::InitAll(ID3D11Device* device)
{
	// �萔�o�b�t�@�[�̍쐬
	constantBuffers.assign({ &sceneParameter, &modelParameter, &lightParameter, &fogParameter, /*&crushParameter*/});
	for (auto& constantBuffer : constantBuffers) {
		constantBuffer->CreateBuffer(device);
	}

	// �e�V�F�[�_�[�̍쐬
	auto hr = device->CreateVertexShader(g_VertexShader, ARRAYSIZE(g_VertexShader), nullptr, vertexShader.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"���_�V�F�[�_�[�̍쐬�Ɏ��s\n");
		return false;
	}
	hr = device->CreateGeometryShader(g_GeometryShader, ARRAYSIZE(g_GeometryShader), nullptr, geometryShader.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"�W�I���g���V�F�[�_�[�̍쐬�Ɏ��s\n");
		return false;
	}
	hr = device->CreatePixelShader(g_PixelShader, ARRAYSIZE(g_PixelShader), nullptr, pixelShader.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"�s�N�Z���V�F�[�_�[�̍쐬�Ɏ��s\n");
		return false;
	}

	// �C���v�b�g���C�A�E�g�̍쐬
	hr = device->CreateInputLayout(VertexPositionNormalTextureColor::inputLayout, ARRAYSIZE(VertexPositionNormalTextureColor::inputLayout),
		g_VertexShader, ARRAYSIZE(g_VertexShader), inputLayout.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"�C���v�b�g���C�A�E�g�̍쐬�Ɏ��s\n");
		return false;
	}

	return true;
}

// �r���[�s��̐ݒ�
void Effect::SetViewMatrix(const DirectX::XMMATRIX& view)
{
	auto& sceneParameter = this->sceneParameter;
	XMStoreFloat4x4(&sceneParameter.GetData().view, XMMatrixTranspose(view));
}

// �v���W�F�N�V�����s��̐ݒ�
void Effect::SetProjectionMatrix(const DirectX::XMMATRIX& projection)
{
	auto& sceneParameter = this->sceneParameter;
	XMStoreFloat4x4(&sceneParameter.GetData().projection, XMMatrixTranspose(projection));
}

// �O��̃t���[������̌o�ߎ��Ԃ̐ݒ�
void Effect::SetTime(float time)
{
	auto& sceneParameter = this->sceneParameter;
	sceneParameter.GetData().time = time;
}

void Effect::SetRandom(int random)
{
	auto& sceneParameter = this->sceneParameter;
	sceneParameter.GetData().randomTest = random;
}

// ���[���h�s��̐ݒ�
void Effect::SetWorldMatrix(const XMMATRIX& matrix)
{
	auto& model = modelParameter;
	XMStoreFloat4x4(&model.GetData().world, XMMatrixTranspose(matrix));
}

// �}�e���A���̐ݒ�
void Effect::SetMaterial(const Material& material)
{
	auto& model = modelParameter;
	model.GetData().material = material;
}

// �f�B���N�V���i�����C�g�̐ݒ�
void Effect::SetDirectionalLight(UINT index, const DirectionalLight& directionalLight)
{
	lightParameter.GetData().directionalLight[index]= directionalLight;
}

// ���_�̐ݒ�
void Effect::SetEyePosition(const DirectX::XMFLOAT3& position)
{
	lightParameter.GetData().eyePosition = position;
}

// �t�H�O�ɂ��Ă̐ݒ�
void Effect::SetFogColor(const XMFLOAT3& color)
{
	auto& fogParameter = this->fogParameter;
	fogParameter.GetData().fogColor = color;
}

void Effect::SetFogState(bool enable)
{
	auto& fogParameter = this->fogParameter;
	fogParameter.GetData().fogEnable = enable;
}

void Effect::SetFogStart(float start)
{
	auto& fogParameter = this->fogParameter;
	fogParameter.GetData().fogStart = start;
}

void Effect::SetFogRange(float range)
{
	auto& fogParameter = this->fogParameter;
	fogParameter.GetData().fogRange = range;
}

// ���ӃG�t�F�N�g�ɂ��Ă̐ݒ�
void Effect::SetInitialVelocity(const DirectX::XMFLOAT4 initialVelocity)
{
	auto& crushParameter = this->crushParameter;
	crushParameter.GetData().initialVelocity = initialVelocity;
}

void Effect::SetIntencity(float intencity)
{
	auto& crushParameter = this->crushParameter;
	crushParameter.GetData().intencity = intencity;
}

// �f�t�H���g�̕`��
void Effect::RenderDefault(ID3D11DeviceContext* immediateContext)
{
	immediateContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	immediateContext->GSSetShader(geometryShader.Get(), nullptr, 0);
	immediateContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	immediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->IASetInputLayout(inputLayout.Get());
	ID3D11SamplerState* samplerStates[1] = { RenderState::linerSamplerState.Get() };
	immediateContext->PSSetSamplers(0, 1, samplerStates);
}

// �萔�o�b�t�@�[�ƃe�N�X�`�����̓K��
void Effect::Apply(ID3D11DeviceContext* immediateContext)
{
	auto& constantBuffer = constantBuffers;
	constantBuffer[static_cast<int>(Data::Scene)]->BindGS(immediateContext);
	constantBuffer[static_cast<int>(Data::Model)]->BindGS(immediateContext);
	constantBuffer[static_cast<int>(Data::Model)]->BindPS(immediateContext);
	constantBuffer[static_cast<int>(Data::Light)]->BindPS(immediateContext);
	constantBuffer[static_cast<int>(Data::Fog)]->BindPS(immediateContext);
	//constantBuffer[static_cast<int>(Data::Crush)]->BindGS(immediateContext);
	//constantBuffer[static_cast<int>(Data::Crush)]->BindPS(immediateContext);

	for (auto& buffer : constantBuffers) {
		buffer->UpdateBuffer(immediateContext);
	}
}