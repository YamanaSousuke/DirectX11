
// �f�B���N�V���i�����C�g�̌�
static const int numDirectionalLight = 4;

// �萔�o�b�t�@�[
cbuffer ConstantBuffer
{
	matrix world;					// ���[���h�s��
	matrix view;					// �r���[�s��
	matrix projection;				// �v���W�F�N�V�����s��
	matrix worldViewProjection;		// WVP�s��
	float time;
};

// �f�B���N�V���i�����C�g
struct DirectionalLight {
	float4 lightColor;
	float4 lightDirection;
};

// ���C�g
cbuffer LightParameter {
	DirectionalLight directionalLight[4];
	float4 eyePosition;				// ���_
};

// ���_�V�F�[�_�[�ւ̓���
struct VSInput
{
	float4 position : POSITION;
	float4 worldPosition : POSITION;
    // float3 normal : NORMAL;
    // float2 texCoord : TEXCOORD;
};

// ���_�V�F�[�_�[����̏o��
struct VSOutput
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	// float3 normal : NORMAL;
	// float2 texCoord : TEXCOORD;
};

// �W�I���g���V�F�[�_�[�ւ̓���
typedef VSOutput GSInput;

// �W�I���g���V�F�[�_�[����̓���
struct GSOutput
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	// float3 normal : NORMAL;
	// float2 texCoord : TEXCOORD;
};

// �s�N�Z���V�F�[�_�[�ւ̓���
typedef GSOutput PSInput;