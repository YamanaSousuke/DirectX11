#pragma once

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Lightings.h"

// ポリゴンに対してエフェクトをする
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

	// 定数バッファーとテクスチャ情報の適応
	void Apply(ID3D11DeviceContext* immediateContext);


private:
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

	enum class Data
	{
		Scene,
		Model,
	};

	ConstantBufferObject<0, SceneParameter> sceneParameter = {};
	ConstantBufferObject<1, ModelParameter> modelParameter = {};

	// 定数バッファーをまとめて管理する
	std::vector<ConstantBufferBase*> constantBuffers = { nullptr };
};
