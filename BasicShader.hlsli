// �萔�o�b�t�@�[
cbuffer ConstantBuffer
{
	matrix world;					// ���[���h�s��
	matrix view;					// �r���[�s��
	matrix projection;				// �v���W�F�N�V�����s��
	matrix worldViewProjection;		// WVP�s��
	float time;
	float alpha;
};

// ���_�V�F�[�_�[�ւ̓���
struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

// ���_�V�F�[�_�[����̏o��
struct VSOutput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

// �W�I���g���V�F�[�_�[�ւ̓���
typedef VSOutput GSInput;

// �W�I���g���V�F�[�_�[����̓���
struct GSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD0;
};

// �s�N�Z���V�F�[�_�[�ւ̓���
typedef GSOutput PSInput;