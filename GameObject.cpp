#include "GameObject.h"

using namespace DirectX;

// トランスフォームの取得
Transform& GameObject::GetTransform()
{
	return transform;
}

// テクスチャの設定
void GameObject::SetTexture(ID3D11ShaderResourceView* texture)
{
	this->texture = texture;
}

// 座標の設定
void GameObject::SetPosition(const XMFLOAT3& position)
{
	this->position = position;
}

// 描画
void GameObject::Draw(ID3D11DeviceContext* immediateContext)
{
	// 頂点バッファーとインデックスバッファーの設定
	ID3D11Buffer* vertexBuffers[1] = { vertexBuffer.Get()};
	UINT strides[1] = { vertexStride };
	UINT offsets[1] = { 0 };
	immediateContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, strides, offsets);
	immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	ModelParameter modelParameter = {};
	static float time = 0.0f;
	time += 0.01666f;

	// ワールド行列
	XMMATRIX world = XMMatrixIdentity();
	XMVECTOR axis = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);
	world *= XMMatrixRotationAxis(axis, time * 0.5f);

	auto position = transform.GetPosition();
	world *= XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&modelParameter.world, XMMatrixTranspose(world));

	// 定数バッファーを取得して、データの転送を行う
	ComPtr<ID3D11Buffer> constnatBuffer = nullptr;
	immediateContext->GSGetConstantBuffers(1, 1, constnatBuffer.GetAddressOf());
	immediateContext->UpdateSubresource(constnatBuffer.Get(), 0, nullptr, &modelParameter, 0, 0);

	// テクスチャの設定
	ID3D11ShaderResourceView* textureViews[1] = { texture.Get() };
	immediateContext->PSSetShaderResources(0, 1, textureViews);
	immediateContext->DrawIndexed(indexCount, 0, 0);
}

// リソースの解放
void GameObject::Release()
{
	delete this;
}
