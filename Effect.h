#pragma once

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Lightings.h"

// 定数バッファーを介してエフェクトを行う
class Effect
{
public:
	// リソースの初期化
	bool InitAll(ID3D11Device* device);

	// ビュー行列の設定
	void SetViewMatrix(const DirectX::XMVECTOR& eye, const DirectX::XMVECTOR& focus, const DirectX::XMVECTOR& up);
	// プロジェクション行列の設定
	void SetProjectionMatrix(float fov, float aspect, float nearZ, float farZ);
	// ワールド行列の設定
	void SetWorldMatrix(const DirectX::XMMATRIX& matrix);
	// マテリアルの設定
	void SetMaterial(const Material& material);
	// ディレクショナルライトの設定
	void SetDirectionalLight(UINT index, const DirectionalLight& directionalLight);
	// 視点の設定
	void SetEyePosition(const DirectX::XMFLOAT3& position);
	// 定数バッファーとテクスチャ情報の適応
	void Apply(ID3D11DeviceContext* immediateContext);

	// デフォルトの描画
	void RenderDefault(ID3D11DeviceContext* immediateContext);
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// シーン情報
	struct SceneParameter {
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	// モデル情報
	struct ModelParameter {
		DirectX::XMFLOAT4X4 world;
		Material material = {};
	};

	// ライト情報
	struct LightParameter {
		DirectionalLight directionalLight[4] = {};
		DirectX::XMFLOAT3 eyePosition;
	};

	enum class Data
	{
		Scene,
		Model,
		Light,
	};

	ConstantBufferObject<0, SceneParameter> sceneParameter = {};
	ConstantBufferObject<1, ModelParameter> modelParameter = {};
	ConstantBufferObject<2, LightParameter> lightParameter = {};

	// 定数バッファーをまとめて管理する
	std::vector<ConstantBufferBase*> constantBuffers = { nullptr };

	// 頂点シェーダー
	ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	// ジオメトリシェーダー
	ComPtr<ID3D11GeometryShader> geometryShader = nullptr;
	// ピクセルシェーダー
	ComPtr<ID3D11PixelShader> pixelShader = nullptr;
	// インプットレイアウト
	ComPtr<ID3D11InputLayout> inputLayout = nullptr;
};
