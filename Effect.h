#pragma once

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Lightings.h"
#include "RenderState.h"

// シェーダーと定数バッファーを管理する
class Effect
{
public:
	// リソースの初期化
	bool InitAll(ID3D11Device* device);
	// ビュー行列の設定
	void SetViewMatrix(const DirectX::XMMATRIX& view);
	// プロジェクション行列の設定
	void SetProjectionMatrix(const DirectX::XMMATRIX& projection);
	// 前回のフレームからの経過時間の設定
	void SetTime(float time);

	void SetRandom(int random);
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

	// フォグについての設定
	void SetFogColor(const DirectX::XMFLOAT3& color);
	void SetFogState(bool enable);
	void SetFogStart(float start);
	void SetFogRange(float range);

	// 粉砕エフェクトについての設定
	void SetInitialVelocity(const DirectX::XMFLOAT4 initialVelocity);
	void SetIntencity(float intencity);

	// デフォルトの描画
	void RenderDefault(ID3D11DeviceContext* immediateContext);
	// 法線の描画
	void ShowNormaLine(ID3D11DeviceContext* immediateContext);
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// シーン情報
	struct SceneParameter {
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		float time;
		int randomTest;
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

	// フォグ
	struct FogParameter {
		DirectX::XMFLOAT3 fogColor;
		int fogEnable;
		float fogStart;
		float fogRange;
	};

	// 粉砕エフェクト
	struct CrushParamater {
		DirectX::XMFLOAT4 initialVelocity;
		float intencity;
	};

	// 定数バッファーで転送するデータ
	enum class Data
	{
		Scene,
		Model,
		Light,
		Fog,
		Crush,
	};

	ConstantBufferObject<0, SceneParameter> sceneParameter = {};
	ConstantBufferObject<1, ModelParameter> modelParameter = {};
	ConstantBufferObject<2, LightParameter> lightParameter = {};
	ConstantBufferObject<3, FogParameter> fogParameter = {};
	ConstantBufferObject<4, CrushParamater> crushParameter = {};

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
