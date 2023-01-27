
// ディレクショナルライトの個数
static const int numDirectionalLight = 4;

// 定数バッファー
cbuffer ConstantBuffer
{
	matrix world;					// ワールド行列
	matrix view;					// ビュー行列
	matrix projection;				// プロジェクション行列
	matrix worldViewProjection;		// WVP行列
	float time;
};

// ディレクショナルライト
struct DirectionalLight {
	float4 lightColor;
	float4 lightDirection;
};

// ライト
cbuffer LightParameter {
	DirectionalLight directionalLight[4];
	float4 eyePosition;				// 視点
};

// 頂点シェーダーへの入力
struct VSInput
{
	float4 position : POSITION;
	float4 worldPosition : POSITION;
	float3 normal : NORMAL;
};

// 頂点シェーダーからの出力
struct VSOutput
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float3 normal : NORMAL;
};

// ジオメトリシェーダーへの入力
typedef VSOutput GSInput;

// ジオメトリシェーダーからの入力
struct GSOutput
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float3 normal : NORMAL;
};

// ピクセルシェーダーへの入力
typedef GSOutput PSInput;