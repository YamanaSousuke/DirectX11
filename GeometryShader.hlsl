#include "BasicShader.hlsli"

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
		element.position = mul(input[i].position, world);
		element.worldPosition = mul(input[i].position, world);
		element.position = mul(element.position, view);
		element.position = mul(element.position, projection);
		element.color = input[i].color;
		element.normal = normalize(mul(input[i].normal, (float3x3)world));
		// element.texCoord = input[i].texCoord;
		output.Append(element);
	}
}