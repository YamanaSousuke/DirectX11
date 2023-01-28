
// ディレクショナルライトの個数
static const int numDirectionalLight = 4;

Texture2D diffuseTexture;
SamplerState diffuseSampler;

cbuffer ModelParameter : register(b1) {
	matrix modelWorld;
};

// 定数バッファー
cbuffer ConstantBuffer : register(b0) {
	matrix view;
	matrix projection;
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
struct VSInput {
	float4 position : POSITION;
	float4 worldPosition : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

// 頂点シェーダーからの出力
struct VSOutput {
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// ジオメトリシェーダーへの入力
typedef VSOutput GSInput;

// ジオメトリシェーダーからの入力
struct GSOutput {
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// ピクセルシェーダーへの入力
typedef GSOutput PSInput;