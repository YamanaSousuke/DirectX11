#include "BasicShader.hlsli"

static const float4 random[8] = {
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ -1.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, -1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, -1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f, 1.0f },
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
		element.position = mul(input[i].position, world);
		element.worldPosition = element.position;

		// 重力加速度
		// element.position = 0.5f * gravity * time * time + initialVelocity * time + element.position;

		element.position = mul(element.position, view);
		element.position = mul(element.position, projection);
		element.color = input[i].color;
		element.normal = normalize(mul(input[i].normal, (float3x3)world));
		// element.position = float4(element.normal, 1.0f) * random[id * randomTest % random.Length] * time * intencity + element.position;

		element.texCoord = input[i].texCoord;
		output.Append(element);
	}
}