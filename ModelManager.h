#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <string_view>
#include <string.h>

class Model
{
public:
private:
};

class ModelManager
{
public:
	// ������
	void Init(ID3D11Device* device);

	Model* CreateFromFile(std::string_view filename);
	Model* CreateFromFile(std::string_view name, std::string_view filename);
private:
	// �f�o�C�X
	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext = nullptr;
};

