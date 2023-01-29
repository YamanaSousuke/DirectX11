#include "BasicShader.hlsli"

static const float roughness = 0.5f;

static const float microfacet = 0.2f;

// フレネル反射を考慮した拡散反射率の計算
float CalcDiffuse(float3 normal, float3 light, float3 view)
{
	float halfVector = (light + view);

	float energyBias = lerp(0.0f, 0.5f, roughness);
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);

	float dotLH = saturate(dot(light, halfVector));

	// 光が平行に入射したときの拡散反射率
	float fd90 = energyBias + 2.0f * dotLH * dotLH * roughness;

	// 法線と光源に向かうベクトルの拡散反射率
	float dotNL = saturate(dot(normal, light));
	float fl = (1 + (fd90 - 1) * pow(1 - dotNL, 5));

	// 法線と視線に向かうベクトルの拡散反射率
	float dotNV = saturate(dot(normal, view));
	float fv = (1 + (fd90 - 1) * pow(1 - dotNV, 5));
	return (fl * fv * energyFactor);
}

// ベックマン分布(マイクロファセット法線の分布)
float Beckmann(float m, float cos)
{
	float cos2 = cos * cos;
	float cos4 = cos * cos * cos * cos;
	float microfacet2 = m * m;
	float d = 1.0f / (4.0f * microfacet2 * cos4);
	// cosからtanに変換する
	d *= exp((-1.0f / microfacet2) * (1.0f - cos2) / cos2);
	return d;
}

// フレネルの計算(Schlick近似)
float Fresnel(float f0, float dotVH)
{
	return f0 + (1 - f0) * pow(1 - dotVH, 5);
}

// 幾何減衰(シャドウイングとマスキングを考慮)
float GeometricDamping(float dotNH, float dotNV, float dotVH, float dotNL)
{
	return min(1.0f, min(2 * dotNH * dotNV / dotVH, 2 * dotNH * dotNL / dotVH));
}

// Cook-Trranceモデルの鏡面反射
float CookTrranceSpecular(float3 light, float3 view, float3 normal, float metallic)
{
	float halfVector = normalize(light + view);
	float f0 = metallic;

	// 内積の計算
	float dotNH = saturate(dot(normal, halfVector));
	float dotVH = saturate(dot(view, halfVector));
	float dotNL = saturate(dot(normal, light));
	float dotNV = saturate(dot(normal, view));

	// D項
	float d = Beckmann(microfacet, dotNH);
	// F項
	float f = Fresnel(f0, dotVH);
	// G項
	float g = GeometricDamping(dotNH, dotNV, dotVH, dotNL);
	// m項
	float m = dotNV * dotNH * PI;
	return max(f * d * g / m, 0.0f);

}

// ピクセルシェーダー
float4 main(PSInput input) : SV_TARGET
{
	float4 albedoColor = diffuseTexture.Sample(diffuseSampler, input.texCoord);

	float3 specularColor = albedoColor;
	// 視線ベクトル
	float3 toEye = normalize(eyePosition - input.worldPosition);
	
	float3 light = 0.0f;
	[unroll]
	for (int i = 0; i < numDirectionalLight; i++)
	{
		// BRDF拡散反射
		float4 lightVector = normalize(directionalLight[i].lightDirection);
		float diffuseFresnel = CalcDiffuse(input.normal, lightVector, toEye);

		// 正規化ランバート反射
		float dotNL = saturate(dot(lightVector.xyz, input.normal));
		float3 lambertDiffuse = directionalLight[i].lightColor.xyz * dotNL / PI;
		float3 diffuse = material.materialDiffuse.xyz * lambertDiffuse * diffuseFresnel * albedoColor;

		// BRDF鏡面反射
		float3 specular = CookTrranceSpecular(lightVector, toEye, input.normal, material.smooth) * directionalLight[i].lightColor.xyz;

		// 金属度が高ければスぺキュラ反射には色が付く、低ければ白
		specular *= lerp(float3(1.0f, 1.0f, 1.0f), specularColor, material.metallic);
		// 拡散反射と鏡面反射の合成
		light += diffuse * (1.0f - material.smooth) + specular;
	}

	light += ambient * albedoColor;
	return float4(light.xyz, 1.0f);
	// return float4(testSpecular, testSpecular, testSpecular, 1.0f);

}