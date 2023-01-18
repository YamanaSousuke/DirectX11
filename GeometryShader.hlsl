#include "BasicShader.hlsli"

// マテリアルの表面カラー
static const float4 diffuseColor = float4(1.0f, 1.0f, 0.0f, 1.0f);
// 重力
static const float4 gravity = float4(0.0f, -9.8f, 0.0f, 0.0f);
// 初速度
static const float4 initialVelocity = float4(0.0f, 5.0f, 0.0f, 0.0f);
// 破裂するときの強さ
static const float intencity = 6.0f;
// 疑似的な乱数
static const float4 random[4] = {
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
};

// エントリーポイント
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
		// ワールド変換
		float4 position = mul(input[i].position, world);
		// 重力落下運動
		position = 0.5f * gravity * time * time + initialVelocity * time + position;
		// ビュープロジェクション変換
		position = mul(position, view);
		element.position = mul(position, projection);

		// ポリゴンごとに分割する
	    float3 worldNormal = normalize(mul(input[i].normal, (float3x3)world));
	    element.position = float4(worldNormal, 1.0f) * random[id % random.Length] * time * intencity + element.position;

		// テクスチャ情報は変更しない
		element.texCoord = input[i].texCoord;
		element.color = float4(diffuseColor.xyz, alpha);
		output.Append(element);
	}
}