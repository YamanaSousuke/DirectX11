
Texture2D diffuseTexture;
SamplerState diffuseSampler;

// ディレクショナルライトの個数
static const int numDirectionalLight = 4;
// 環境光
static const float ambient = 0.2f;
// π
static const float PI = 3.1415926f;

// ディレクショナルライト
struct DirectionalLight {
	float4 lightColor;
	float4 lightDirection;
};

// マテリアル
struct Material {
	float4 materialDiffuse;
	float smooth;
	float metallic;
};

// シーン情報
cbuffer SceneParameter : register(b0) {
	matrix view;
	matrix projection;
};

// モデル情報
cbuffer ModelParameter : register(b1) {
	matrix world;
	Material material;
};

// ライト
cbuffer LightParameter : register(b2) {
	DirectionalLight directionalLight[numDirectionalLight];
	float3 eyePosition;
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