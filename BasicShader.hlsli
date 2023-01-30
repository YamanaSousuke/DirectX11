
Texture2D diffuseTexture;
SamplerState diffuseSampler;

// �f�B���N�V���i�����C�g�̌�
static const int numDirectionalLight = 4;
// ����
static const float ambient = 0.2f;
// ��
static const float PI = 3.1415926f;

// �f�B���N�V���i�����C�g
struct DirectionalLight {
	float4 lightColor;
	float4 lightDirection;
};

// �}�e���A��
struct Material {
	float4 materialDiffuse;
	float smooth;
	float metallic;
};

// �V�[�����
cbuffer SceneParameter : register(b0) {
	matrix view;
	matrix projection;
};

// ���f�����
cbuffer ModelParameter : register(b1) {
	matrix world;
	Material material;
};

// ���C�g
cbuffer LightParameter : register(b2) {
	DirectionalLight directionalLight[numDirectionalLight];
	float3 eyePosition;
};

// ���_�V�F�[�_�[�ւ̓���
struct VSInput {
	float4 position : POSITION;
	float4 worldPosition : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

// ���_�V�F�[�_�[����̏o��
struct VSOutput {
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// �W�I���g���V�F�[�_�[�ւ̓���
typedef VSOutput GSInput;

// �W�I���g���V�F�[�_�[����̓���
struct GSOutput {
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// �s�N�Z���V�F�[�_�[�ւ̓���
typedef GSOutput PSInput;