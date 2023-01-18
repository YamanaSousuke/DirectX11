#include "BasicShader.hlsli"

// �}�e���A���̕\�ʃJ���[
static const float4 diffuseColor = float4(1.0f, 1.0f, 0.0f, 1.0f);
// �d��
static const float4 gravity = float4(0.0f, -9.8f, 0.0f, 0.0f);
// �����x
static const float4 initialVelocity = float4(0.0f, 5.0f, 0.0f, 0.0f);
// �j�􂷂�Ƃ��̋���
static const float intencity = 6.0f;
// �^���I�ȗ���
static const float4 random[4] = {
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
};

// �G���g���[�|�C���g
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
		position = 0.5f * gravity * time * time + initialVelocity * time + position;
		// �r���[�v���W�F�N�V�����ϊ�
		position = mul(position, view);
		element.position = mul(position, projection);

		// �|���S�����Ƃɕ�������
	    float3 worldNormal = normalize(mul(input[i].normal, (float3x3)world));
	    element.position = float4(worldNormal, 1.0f) * random[id % random.Length] * time * intencity + element.position;

		// �e�N�X�`�����͕ύX���Ȃ�
		element.texCoord = input[i].texCoord;
		element.color = float4(diffuseColor.xyz, alpha);
		output.Append(element);
	}
}