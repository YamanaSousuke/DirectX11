#include "BasicShader.hlsli"

static const float roughness = 0.5f;

static const float microfacet = 0.2f;

// �t���l�����˂��l�������g�U���˗��̌v�Z
float CalcDiffuse(float3 normal, float3 light, float3 view)
{
	float halfVector = (light + view);

	float energyBias = lerp(0.0f, 0.5f, roughness);
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);

	float dotLH = saturate(dot(light, halfVector));

	// �������s�ɓ��˂����Ƃ��̊g�U���˗�
	float fd90 = energyBias + 2.0f * dotLH * dotLH * roughness;

	// �@���ƌ����Ɍ������x�N�g���̊g�U���˗�
	float dotNL = saturate(dot(normal, light));
	float fl = (1 + (fd90 - 1) * pow(1 - dotNL, 5));

	// �@���Ǝ����Ɍ������x�N�g���̊g�U���˗�
	float dotNV = saturate(dot(normal, view));
	float fv = (1 + (fd90 - 1) * pow(1 - dotNV, 5));
	return (fl * fv * energyFactor);
}

// �x�b�N�}�����z(�}�C�N���t�@�Z�b�g�@���̕��z)
float Beckmann(float m, float cos)
{
	float cos2 = cos * cos;
	float cos4 = cos * cos * cos * cos;
	float microfacet2 = m * m;
	float d = 1.0f / (4.0f * microfacet2 * cos4);
	// cos����tan�ɕϊ�����
	d *= exp((-1.0f / microfacet2) * (1.0f - cos2) / cos2);
	return d;
}

// �t���l���̌v�Z(Schlick�ߎ�)
float Fresnel(float f0, float dotVH)
{
	return f0 + (1 - f0) * pow(1 - dotVH, 5);
}

// �􉽌���(�V���h�E�C���O�ƃ}�X�L���O���l��)
float GeometricDamping(float dotNH, float dotNV, float dotVH, float dotNL)
{
	return min(1.0f, min(2 * dotNH * dotNV / dotVH, 2 * dotNH * dotNL / dotVH));
}

// Cook-Trrance���f���̋��ʔ���
float CookTrranceSpecular(float3 light, float3 view, float3 normal, float metallic)
{
	float halfVector = normalize(light + view);
	float f0 = metallic;

	// ���ς̌v�Z
	float dotNH = saturate(dot(normal, halfVector));
	float dotVH = saturate(dot(view, halfVector));
	float dotNL = saturate(dot(normal, light));
	float dotNV = saturate(dot(normal, view));

	// D��
	float d = Beckmann(microfacet, dotNH);
	// F��
	float f = Fresnel(f0, dotVH);
	// G��
	float g = GeometricDamping(dotNH, dotNV, dotVH, dotNL);
	// m��
	float m = dotNV * dotNH * PI;
	return max(f * d * g / m, 0.0f);

}

// �s�N�Z���V�F�[�_�[
float4 main(PSInput input) : SV_TARGET
{
	float4 albedoColor = diffuseTexture.Sample(diffuseSampler, input.texCoord);

	float3 specularColor = albedoColor;
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
		float3 diffuse = material.materialDiffuse.xyz * lambertDiffuse * diffuseFresnel * albedoColor;

		// BRDF���ʔ���
		float3 specular = CookTrranceSpecular(lightVector, toEye, input.normal, material.smooth) * directionalLight[i].lightColor.xyz;

		// �����x��������΃X�؃L�������˂ɂ͐F���t���A�Ⴏ��Δ�
		specular *= lerp(float3(1.0f, 1.0f, 1.0f), specularColor, material.metallic);
		// �g�U���˂Ƌ��ʔ��˂̍���
		light += diffuse * (1.0f - material.smooth) + specular;
	}

	light += ambient * albedoColor;
	return float4(light.xyz, 1.0f);
	// return float4(testSpecular, testSpecular, testSpecular, 1.0f);

}