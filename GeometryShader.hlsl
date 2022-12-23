#include "BasicShader.hlsli"

// ライトの位置座標
static const float4 lightPosition = float4(1.0f, 2.0f, -2.0f, 0.0f);
// マテリアルの表面カラー
static const float4 diffuseColor = float4(1.0f, 1.0f, 0.0f, 1.0f);
// 重力
static const float4 gravity = float4(0.0f, -9.8f, 0.0f, 0.0f);
// 初速度
static const float4 initialVelocity = float4(0.0f, 5.0f, 0.0f, 0.0f);
// 破裂するときの強さ
static const float intencity = 6.0f;

static const float4 random[3] = {
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f },
};

[maxvertexcount(3)]
void main(
	triangle GSInput input[3], 
	in uint id : SV_PrimitiveID,
	inout TriangleStream< GSOutput > output
)
{
	[unroll]
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;

		float4 randomVector = random[id % random.Length];

		// ワールド変換
		float4 position = mul(input[i].position, world);
		// 重力落下運動
		// position = 0.5f * gravity * time.x * time.x + initialVelocity * time.x + position;
		position = 0.5f * gravity * time * time + initialVelocity * time + position;

		// position = float4(randomNormal, 1.0f) * time.x + position;

		// ビュー変換
		position = mul(position, view);
		// プロジェクション変換
		element.position = mul(position, projection);

		float3 normal = normalize(mul(input[i].normal, (float3x3)world));

		// element.position = float4(normal, 1.0f) * randomVector * time.x * intencity + element.position;
		element.position = float4(normal, 1.0f) * randomVector * time * intencity + element.position;

		// 光源ベクトル
		float3 light = normalize(lightPosition.xyz);
		// ワールド空間上の法線ベクトル
		float3 worldNormal = normalize(mul(input[i].normal, (float3x3)world));
		// ランバート反射
		float diffuse = max(dot(light, worldNormal), 0);
		element.color = float4(diffuse * diffuseColor.xyz, alpha);
		output.Append(element);
	}
}