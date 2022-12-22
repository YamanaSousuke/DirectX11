#include "BasicShader.hlsli"

// ライトの位置座標
static const float4 lightPosition = float4(1.0f, 2.0f, -2.0f, 0.0f);
// マテリアルの表面カラー
static const float4 diffuseColor = float4(1.0f, 1.0f, 0.0f, 1.0f);

[maxvertexcount(3)]
void main(
	triangle GSInput input[3], 
	inout TriangleStream< GSOutput > output
)
{
	[unroll]
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.position = mul(input[i].position, worldViewProjection);
		float3 light = normalize(lightPosition.xyz);
		// ワールド空間上の法線ベクトル
		float3 worldNormal = normalize(mul(input[i].normal, (float3x3)world));
		// ランバート反射
		float diffuse = max(dot(light, worldNormal), 0);
		element.color = float4(diffuse * diffuseColor.xyz, diffuseColor.w);
		output.Append(element);
	}
}