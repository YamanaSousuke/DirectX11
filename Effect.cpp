#include "Effect.h"
#include "Vertex.h"

#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

using namespace DirectX;

// リソースの初期化
bool Effect::InitAll(ID3D11Device* device)
{
	// 定数バッファーの作成
	constantBuffers.assign({ &sceneParameter, &modelParameter, &lightParameter, /*&fogParameter*/});
	for (auto& constantBuffer : constantBuffers) {
		constantBuffer->CreateBuffer(device);
	}

	// 各シェーダーの作成
	auto hr = device->CreateVertexShader(g_VertexShader, ARRAYSIZE(g_VertexShader), nullptr, vertexShader.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"頂点シェーダーの作成に失敗\n");
		return false;
	}
	hr = device->CreateGeometryShader(g_GeometryShader, ARRAYSIZE(g_GeometryShader), nullptr, geometryShader.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"ジオメトリシェーダーの作成に失敗\n");
		return false;
	}
	hr = device->CreatePixelShader(g_PixelShader, ARRAYSIZE(g_PixelShader), nullptr, pixelShader.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"ピクセルシェーダーの作成に失敗\n");
		return false;
	}

	// インプットレイアウトの作成
	hr = device->CreateInputLayout(VertexPositionNormalTextureColor::inputLayout, ARRAYSIZE(VertexPositionNormalTextureColor::inputLayout),
		g_VertexShader, ARRAYSIZE(g_VertexShader), inputLayout.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"インプットレイアウトの作成に失敗\n");
		return false;
	}

	return true;
}

// ビュー行列の設定
void Effect::SetViewMatrix(const XMVECTOR& eye, const XMVECTOR& focus, const XMVECTOR& up)
{
	auto view = XMMatrixLookAtLH(eye, focus, up);
	auto& sceneParameter = this->sceneParameter;
	XMStoreFloat4x4(&sceneParameter.data.view, XMMatrixTranspose(view));
}

// プロジェクション行列の設定
void Effect::SetProjectionMatrix(float fov, float aspect, float nearZ, float farZ)
{
	auto projection = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
	auto& sceneParameter = this->sceneParameter;
	XMStoreFloat4x4(&sceneParameter.data.projection, XMMatrixTranspose(projection));
}

// ワールド行列の設定
void Effect::SetWorldMatrix(const XMMATRIX& matrix)
{
	auto& model = modelParameter;
	XMStoreFloat4x4(&model.data.world, XMMatrixTranspose(matrix));
}

// マテリアルの設定
void Effect::SetMaterial(const Material& material)
{
	auto& model = modelParameter;
	model.data.material = material;
}

// ディレクショナルライトの設定
void Effect::SetDirectionalLight(UINT index, const DirectionalLight& directionalLight)
{
	lightParameter.data.directionalLight[index]= directionalLight;
}

// 視点の設定
void Effect::SetEyePosition(const DirectX::XMFLOAT3& position)
{
	lightParameter.data.eyePosition = position;
}

// 定数バッファーとテクスチャ情報の適応
void Effect::Apply(ID3D11DeviceContext* immediateContext)
{
	auto& constantBuffer = constantBuffers;
	constantBuffer[static_cast<int>(Data::Scene)]->BindGS(immediateContext);
	constantBuffer[static_cast<int>(Data::Model)]->BindGS(immediateContext);
	constantBuffer[static_cast<int>(Data::Model)]->BindPS(immediateContext);
	constantBuffer[static_cast<int>(Data::Light)]->BindPS(immediateContext);
	// constantBuffer[static_cast<int>(Data::Fog)]->BindPS(immediateContext);

	for (auto& buffer : constantBuffers) {
		buffer->UpdateBuffer(immediateContext);
	}
}

void Effect::SetFogColor(const XMFLOAT3& color)
{
	auto& fogParameter = this->fogParameter;
	fogParameter.data.fogColor = color;
}

void Effect::SetFogState(bool enable)
{
	auto& fogParameter = this->fogParameter;
	fogParameter.data.fogEnable = enable;
}

void Effect::SetFogStart(float start)
{
	auto& fogParameter = this->fogParameter;
	fogParameter.data.fogStart = start;
}

void Effect::SetFogRange(float range)
{
	auto& fogParameter = this->fogParameter;
	fogParameter.data.fogRange = range;
}
// デフォルトの描画
void Effect::RenderDefault(ID3D11DeviceContext* immediateContext)
{
	immediateContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	immediateContext->GSSetShader(geometryShader.Get(), nullptr, 0);
	immediateContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	immediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->IASetInputLayout(inputLayout.Get());
}
