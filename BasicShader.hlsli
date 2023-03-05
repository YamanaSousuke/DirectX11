
Texture2D diffuseTexture;
SamplerState diffuseSampler;

// �f�B���N�V���i�����C�g�̌�
static const int numDirectionalLight = 4;
// ��
static const float PI = 3.1415926f;
// �d��
static const float4 gravity = float4(0.0f, -9.8f, 0.0f, 0.0f);
// �e��
static const float roughness = 0.5f;
// �}�C�N���t�@�Z�b�g
static const float microfacet = 0.76f;
// �g�U���˂̌v�Z���̐��K���p
static const float maxEnergy = 1.5f;

// �f�B���N�V���i�����C�g
struct DirectionalLight {
	float4 lightDiffuse;
	float4 lightAmbient;
	float4 lightDirection;
};

// �}�e���A��
struct Material {
	float4 materialDiffuse;
	float4 materialAmbient;
	float4 materialSpecular;
	float smooth;
	float metallic;
};

// �V�[�����
cbuffer SceneParameter : register(b0) {
	matrix view;
	matrix projection;
	float time;
	int randomTest;
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

// �t�H�O
cbuffer FogParameter : register(b3) {
	float3 fogColor;
	int fogEnable;
	float fogStart;
	float fogRange;
}

// ���ӃG�t�F�N�g
cbuffer CrushParameter : register(b4) {
	float4 initialVelocity;
	float intencity;
}

// ���_�V�F�[�_�[�ւ̓���
struct VSInput {
	float4 position : POSITION;
	float4 worldPosition : POSITION;
	float4 color : COLOR;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

// ���_�V�F�[�_�[����̏o��
struct VSOutput {
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// �W�I���g���V�F�[�_�[�ւ̓���
typedef VSOutput GSInput;

// �W�I���g���V�F�[�_�[����̓���
struct GSOutput {
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// �s�N�Z���V�F�[�_�[�ւ̓���
typedef GSOutput PSInput;