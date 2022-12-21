#include "Game.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

using namespace Microsoft::WRL;

// ���_�V�F�[�_�[��\���V�����C���X�^���X�̍쐬
VertexShader* VertexShader::Create(ID3D11Device* device)
{
	// ���_�V�F�[�_�[�̍쐬
	ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	auto hr = device->CreateVertexShader(g_VertexShader, sizeof(g_VertexShader), NULL, &vertexShader);
	if (FAILED(hr)) {
		return nullptr;
	}

	// �߂�l�Ƃ��ĕԂ��C���X�^���X�̍쐬
	auto result = new VertexShader();
	if (result == nullptr) {
		return nullptr;
	}
	result->vertexShader = vertexShader;
	return result;
}

// ���_�V�F�[�_�[�̃l�C�e�B�u�|�C���^�[�̎擾
ID3D11VertexShader* VertexShader::GetNativePointer()
{
	return vertexShader.Get();
}

// ���\�[�X�̉��
void VertexShader::Release()
{
	vertexShader.Reset();
	delete this;
}

// �W�I���g���V�F�[�_�[��\���V�����C���X�^���X�̍쐬
GeometryShader* GeometryShader::Create(ID3D11Device* device)
{
	// �W�I���g���V�F�[�_�[�̍쐬
	ComPtr<ID3D11GeometryShader> geometryShader = nullptr;
	auto hr = device->CreateGeometryShader(g_GeometryShader, sizeof(g_GeometryShader), NULL, &geometryShader);
	if (FAILED(hr)) {
		return nullptr;
	}

	// �߂�l�Ƃ��ĕԂ��C���X�^���X�̍쐬
	auto result = new GeometryShader();
	if (result == nullptr) {
		return nullptr;
	}
	result->geometryShader = geometryShader;
	return result;
}

// �W�I���g���V�F�[�_�[�̃l�C�e�B�u�|�C���^�[�̎擾
ID3D11GeometryShader* GeometryShader::GetNativePointer()
{
	return geometryShader.Get();
}

// ���\�[�X�̉��
void GeometryShader::Release()
{
	geometryShader.Reset();
	delete this;
}

// �s�N�Z���V�F�[�_�[��\���V�����C���X�^���X�̍쐬
PixelShader* PixelShader::Create(ID3D11Device* device)
{
	// �s�N�Z���V�F�[�_�[�̍쐬
	ComPtr<ID3D11PixelShader> pixelShader = nullptr;
	auto hr = device->CreatePixelShader(g_PixelShader, sizeof(g_PixelShader), NULL, &pixelShader);
	if (FAILED(hr)) {
		return nullptr;
	}

	// �߂�l�Ƃ��ĕԂ��C���X�^���X�̍쐬
	auto result = new PixelShader();
	if (result == nullptr) {
		return nullptr;
	}
	result->pixelShader = pixelShader;
	return result;
}

// �s�N�Z���V�F�[�_�[�̃l�C�e�B�u�|�C���^�[�̎擾
ID3D11PixelShader* PixelShader::GetNativePointer()
{
	return pixelShader.Get();
}

// ���\�[�X�̉��
void PixelShader::Release()
{
	pixelShader.Reset();
	delete this;
}
