
Texture2D diffuseTexture;
SamplerState diffuseSampler;

// ディレクショナルライトの個数
static const int numDirectionalLight = 4;
// π
static const float PI = 3.1415926f;
// 重力
static const float4 gravity = float4(0.0f, -9.8f, 0.0f, 0.0f);
// 粗さ
static const float roughness = 0.5f;
// マイクロファセット
static const float microfacet = 0.76f;
// 拡散反射の計算時の正規化用
static const float maxEnergy = 1.5f;

// ディレクショナルライト
struct DirectionalLight {
	float4 lightDiffuse;
	float4 lightAmbient;
	float4 lightDirection;
};

// マテリアル
struct Material {
	float4 materialDiffuse;
	float4 materialAmbient;
	float4 materialSpecular;
	float smooth;
	float metallic;
};

// シーン情報
cbuffer SceneParameter : register(b0) {
	matrix view;
	matrix projection;
	float time;
	int randomTest;
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

// フォグ
cbuffer FogParameter : register(b3) {
	float3 fogColor;
	int fogEnable;
	float fogStart;
	float fogRange;
}

// 粉砕エフェクト
cbuffer CrushParameter : register(b4) {
	float4 initialVelocity;
	float intencity;
}

// 頂点シェーダーへの入力
struct VSInput {
	float4 position : POSITION;
	float4 worldPosition : POSITION;
	float4 color : COLOR;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

// 頂点シェーダーからの出力
struct VSOutput {
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// ジオメトリシェーダーへの入力
typedef VSOutput GSInput;

// ジオメトリシェーダーからの入力
struct GSOutput {
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// ピクセルシェーダーへの入力
typedef GSOutput PSInput;