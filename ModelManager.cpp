#include "ModelManager.h"

// ‰Šú‰»
void ModelManager::Init(ID3D11Device* device)
{
	this->device = device;
	this->device->GetImmediateContext(immediateContext.ReleaseAndGetAddressOf());
}

Model* ModelManager::CreateFromFile(std::string_view filename)
{
	return CreateFromFile(filename, filename);
}

Model* ModelManager::CreateFromFile(std::string_view name, std::string_view filename)
{
	return nullptr;
}
