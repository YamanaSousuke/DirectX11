#include "BasicShader.hlsli"

// ��
static const float PI = 3.1415926f;
// �}�e���A���̕\�ʃJ���[
static const float4 diffuseColor = float4(1.0f, 1.0f, 0.0f, 1.0f);

static const float testSmooth = 0.0f;
static const float testMetallic = 0.0f;

// �t���l�����˂��l�������g�U���˗��̌v�Z
float CalcDiffuse(float3 normal, float3 light, float3 view)
{
	float dotNL = saturate(dot(normal, light));
	float dotNV = saturate(dot(normal, view));
	return dotNL * dotNV;
}

// �x�b�N�}�����z(�}�C�N���t�@�Z�b�g�@���̕��z)
float Beckmann(float microfacet, float cos)
{
	float cos2 = cos * cos;
	float cos4 = cos * cos * cos * cos;
	float microfacet2 = microfacet * microfacet;
	float d = 1.0f / (4.0f * microfacet2 * cos4);
	// cos����tan�ɕϊ�����
	d *= exp((-1.0f / microfacet2) * (1.0f - cos2) / cos2);
	return d;
}

// �t���l���̌v�Z(Schlick�ߎ�)
float Fresnel(float f0, float dotVH)
{
	return f0 + (1 - f0) * pow(1 - dotVH, 5.0f);
}

// �􉽌���(�V���h�E�C���O�ƃ}�X�L���O���l��)
float GeometricDamping(float dotNH, float dotNV, float dotVH, float dotNL)
{
	return min(1.0f, min(2 * dotNH * dotNV / dotVH, 2 * dotNH * dotNL / dotVH));
}

// Cook-Trrance���f���̋��ʔ���
float CookTrranceSpecular(float3 light, float view, float3 normal, float metallic)
{
	float microfacet = 0.76f;

	float halfVector = normalize(light + view);

	// ���ς̌v�Z
	float dotNH = saturate(dot(normal, halfVector));
	float dotVH = saturate(dot(view, halfVector));

	float dotNL = saturate(dot(normal, light));
	float dotNV = saturate(dot(normal, view));

	// D��
	float d = Beckmann(microfacet, dotNH);

	// F��
	float f = Fresnel(metallic, dotVH);

	// G��
	float g = GeometricDamping(dotNH, dotNV, dotVH, dotNL);

	// m��
	float m = dotNV * dotNL * PI;
	return max(f * d * g / m, 0.0f);

}

// �s�N�Z���V�F�[�_�[
float4 main(PSInput input) : SV_TARGET
{
	// �����x�N�g��
	float3 toEye = normalize(eyePosition - input.worldPosition);
	
	float3 light = 0.0f;
	[unroll]
	for (int i = 0; i < numDirectionalLight; i++)
	{
		// BRDF�g�U����
		float4 lightVector = normalize(directionalLight[i].lightDirection);
		float diffuseFresnel = CalcDiffuse(input.normal, lightVector, toEye);

		// ���K�������o�[�g����
		float dotNL = saturate(dot(lightVector.xyz, input.normal));
		float3 lambertDiffuse = directionalLight[i].lightColor.xyz * dotNL / PI;
		float3 diffuse = diffuseColor.xyz * lambertDiffuse * diffuseFresnel;

		// BRDF���ʔ���
		float specular = CookTrranceSpecular(lightVector, toEye, input.normal, testSmooth) * directionalLight[i].lightColor.xyz;

		// �����x��������΃X�؃L�������˂ɂ͐F���t���A�Ⴏ��Δ�
		specular *= lerp(float3(1.0f, 1.0f, 1.0f), float3(0.0f, 0.0f, 0.0f), testMetallic);
		// �g�U���˂Ƌ��ʔ��˂̍���
		light += diffuse * (1.0f - testSmooth) + specular;
	}

	return float4(light.xyz, 1.0f);
}