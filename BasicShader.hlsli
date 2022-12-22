// 定数バッファー
cbuffer ConstantBuffer
{
	matrix world;					// ワールド行列
	matrix view;					// ビュー行列
	matrix projection;				// プロジェクション行列
	matrix worldViewProjection;		// WVP行列
	float4 time;
};

// 頂点シェーダーへの入力
struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
};

// 頂点シェーダーからの出力
struct VSOutput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

// ジオメトリシェーダーへの入力
typedef VSOutput GSInput;

// ジオメトリシェーダーからの入力
struct GSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// ピクセルシェーダーへの入力
typedef GSOutput PSInput;