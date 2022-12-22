#include "Game.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

using namespace Microsoft::WRL;

// ���_�V�F�[�_�[��\���V�����C���X�^���X�̍쐬
VertexShader* VertexShader::Create(ID3D11Device* device)
{
	// �߂�l�Ƃ��ĕԂ��C���X�^���X�̍쐬
	auto result = new VertexShader();
	if (result == nullptr) {
		return nullptr;
	}

	// ���_�V�F�[�_�[�̍쐬
	auto hr = device->CreateVertexShader(g_VertexShader, _countof(g_VertexShader), NULL, &result->shader);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// �o�C�g�R�[�h�̎擾
const BYTE* VertexShader::GetBytecode()
{
	return g_VertexShader;
}

// �o�C�g�R�[�h�̃T�C�Y�̎擾
SIZE_T VertexShader::GetBytecodeLength()
{
	return _countof(g_VertexShader);
}

// ���_�V�F�[�_�[�̃l�C�e�B�u�|�C���^�[�̎擾
ID3D11VertexShader* VertexShader::GetNativePointer()
{
	return shader.Get();
}

// ���\�[�X�̉��
void VertexShader::Release()
{
	shader.Reset();
	delete this;
}

// �W�I���g���V�F�[�_�[��\���V�����C���X�^���X�̍쐬
GeometryShader* GeometryShader::Create(ID3D11Device* device)
{
	// �߂�l�Ƃ��ĕԂ��C���X�^���X�̍쐬
	auto result = new GeometryShader();
	if (result == nullptr) {
		return nullptr;
	}

	// �W�I���g���V�F�[�_�[�̍쐬
	auto hr = device->CreateGeometryShader(g_GeometryShader, _countof(g_GeometryShader), NULL, &result->shader);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// �W�I���g���V�F�[�_�[�̃l�C�e�B�u�|�C���^�[�̎擾
ID3D11GeometryShader* GeometryShader::GetNativePointer()
{
	return shader.Get();
}

// ���\�[�X�̉��
void GeometryShader::Release()
{
	shader.Reset();
	delete this;
}

// �s�N�Z���V�F�[�_�[��\���V�����C���X�^���X�̍쐬
PixelShader* PixelShader::Create(ID3D11Device* device)
{
	// �߂�l�Ƃ��ĕԂ��C���X�^���X�̍쐬
	auto result = new PixelShader();
	if (result == nullptr) {
		return nullptr;
	}

	// �s�N�Z���V�F�[�_�[�̍쐬
	auto hr = device->CreatePixelShader(g_PixelShader, _countof(g_PixelShader), NULL, &result->shader);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// �s�N�Z���V�F�[�_�[�̃l�C�e�B�u�|�C���^�[�̎擾
ID3D11PixelShader* PixelShader::GetNativePointer()
{
	return shader.Get();
}

// ���\�[�X�̉��
void PixelShader::Release()
{
	shader.Reset();
	delete this;
}
