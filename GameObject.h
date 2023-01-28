#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include "Geometry.h"


// ゲームオブジェクト
class GameObject 
{
public:
	// 頂点バッファーとインデックスバッファーの設定
	template<class VertexType>
	void SetBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext, Geometry::Meshdata<VertexType> meshdata);
	// テクスチャの設定
	void SetTexture(ID3D11ShaderResourceView* texture);
	// 描画
	void Draw(ID3D11DeviceContext* immediateContext, const DirectX::XMFLOAT3& position);
	// リソースの解放
	void Release();
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// モデル情報
	struct ModelParameter {
		DirectX::XMFLOAT4X4 world;
	};

	// 頂点バッファー
	ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	// インデックスバッファー
	ComPtr<ID3D11Buffer> indexBuffer = nullptr;
	// テクスチャー
	ComPtr<ID3D11ShaderResourceView> texture = nullptr;
	// 1頂点のサイズ
	UINT vertexStride = 0;
	// インデックスの個数
	UINT indexCount = 0;
};

template<class VertexType>
inline void GameObject::SetBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext, Geometry::Meshdata<VertexType> meshdata)
{
	vertexStride = sizeof(VertexType);
	indexCount = (UINT)meshdata.indices.size();
	
	// 頂点バッファーの作成
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = vertexStride * (UINT)meshdata.vertices.size();
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	auto hr = device->CreateBuffer(&vertexBufferDesc, NULL, vertexBuffer.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"頂点バッファーの作成に失敗\n");
	}
	immediateContext->UpdateSubresource(vertexBuffer.Get(), 0, nullptr, meshdata.vertices.data(), 0, 0);
	
	// インデックスバッファーの作成
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.ByteWidth = sizeof(UINT16) * indexCount;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	hr = device->CreateBuffer(&indexBufferDesc, NULL, &indexBuffer);
	if (FAILED(hr)) {
		OutputDebugString(L"インデックスバッファーの作成に失敗\n");
	}
	immediateContext->UpdateSubresource(indexBuffer.Get(), 0, nullptr, meshdata.indices.data(), 0, 0);
}
