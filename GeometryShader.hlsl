#include "BasicShader.hlsli"

// ���C�g�̈ʒu���W
static const float4 lightPosition = float4(1.0f, 2.0f, -2.0f, 0.0f);
// �}�e���A���̕\�ʃJ���[
static const float4 diffuseColor = float4(1.0f, 1.0f, 0.0f, 1.0f);
// �d��
static const float4 gravity = float4(0.0f, -9.8f, 0.0f, 0.0f);
// �����x
static const float4 initialVelocity = float4(0.0f, 6.0f, 0.0f, 0.0f);

static const float4 random[3] = {
	{ 1.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f, 0.0f },
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

		// ���[���h�ϊ�
		float4 position = mul(input[i].position, world);
		// �d�͗����^��
		position = 0.5f * gravity * time.x * time.x + initialVelocity * time.x + position;
		// �r���[�ϊ�
		position = mul(position, view);
		// �v���W�F�N�V�����ϊ�
		element.position = mul(position, projection);

		float3 light = normalize(lightPosition.xyz);
		// ���[���h��ԏ�̖@���x�N�g��
		float3 worldNormal = normalize(mul(input[i].normal, (float3x3)world));

		float3 randomNormal = random[id % 3];
		element.position = float4(randomNormal, 1.0f) * time.x * 3.0f + element.position * time.x;

		// �����o�[�g����
		float diffuse = max(dot(light, worldNormal), 0);
		element.color = float4(diffuse * diffuseColor.xyz, diffuseColor.w);
		output.Append(element);
	}
}