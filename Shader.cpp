#include "Game.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

using namespace Microsoft::WRL;

//=============================================================================
// ���_�V�F�[�_�[
//=============================================================================

// ���_�V�F�[�_�[��\���V�����C���X�^���X�̍쐬
VertexShader::VertexShader(ID3D11Device* device)
{
	// ���_�V�F�[�_�[�̍쐬
	auto hr = device->CreateVertexShader(g_VertexShader, _countof(g_VertexShader), NULL, &shader);
	if (FAILED(hr)) {
		shader = nullptr;
	}
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
	delete this;
}

//=============================================================================
// �W�I���g���V�F�[�_�[
//=============================================================================

// �W�I���g���V�F�[�_�[��\���V�����C���X�^���X�̍쐬
GeometryShader::GeometryShader(ID3D11Device* device)
{
	// �W�I���g���V�F�[�_�[�̍쐬
	auto hr = device->CreateGeometryShader(g_GeometryShader, _countof(g_GeometryShader), NULL, &shader);
	if (FAILED(hr)) {
		shader = nullptr;
	}
}

// �W�I���g���V�F�[�_�[�̃l�C�e�B�u�|�C���^�[�̎擾
ID3D11GeometryShader* GeometryShader::GetNativePointer()
{
	return shader.Get();
}

// ���\�[�X�̉��
void GeometryShader::Release()
{
	delete this;
}

//=============================================================================
// �s�N�Z���V�F�[�_�[
//=============================================================================

// �s�N�Z���V�F�[�_�[��\���V�����C���X�^���X�̍쐬
PixelShader::PixelShader(ID3D11Device* device)
{
	// �s�N�Z���V�F�[�_�[�̍쐬
	auto hr = device->CreatePixelShader(g_PixelShader, _countof(g_PixelShader), NULL, &shader);
	if (FAILED(hr)) {
		shader = nullptr;
	}
}

// �s�N�Z���V�F�[�_�[�̃l�C�e�B�u�|�C���^�[�̎擾
ID3D11PixelShader* PixelShader::GetNativePointer()
{
	return shader.Get();
}

// ���\�[�X�̉��
void PixelShader::Release()
{
	delete this;
}
