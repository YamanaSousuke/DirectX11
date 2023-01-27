#include "BasicShader.hlsli"

// π
static const float PI = 3.1415926f;
// マテリアルの表面カラー
static const float4 diffuseColor = float4(1.0f, 1.0f, 0.0f, 1.0f);

static const float testSmooth = 0.0f;
static const float testMetallic = 0.0f;

// フレネル反射を考慮した拡散反射率の計算
float CalcDiffuse(float3 normal, float3 light, float3 view)
{
	float dotNL = saturate(dot(normal, light));
	float dotNV = saturate(dot(normal, view));
	return dotNL * dotNV;
}

// ベックマン分布(マイクロファセット法線の分布)
float Beckmann(float microfacet, float cos)
{
	float cos2 = cos * cos;
	float cos4 = cos * cos * cos * cos;
	float microfacet2 = microfacet * microfacet;
	float d = 1.0f / (4.0f * microfacet2 * cos4);
	// cosからtanに変換する
	d *= exp((-1.0f / microfacet2) * (1.0f - cos2) / cos2);
	return d;
}

// フレネルの計算(Schlick近似)
float Fresnel(float f0, float dotVH)
{
	return f0 + (1 - f0) * pow(1 - dotVH, 5.0f);
}

// 幾何減衰(シャドウイングとマスキングを考慮)
float GeometricDamping(float dotNH, float dotNV, float dotVH, float dotNL)
{
	return min(1.0f, min(2 * dotNH * dotNV / dotVH, 2 * dotNH * dotNL / dotVH));
}

// Cook-Trranceモデルの鏡面反射
float CookTrranceSpecular(float3 light, float view, float3 normal, float metallic)
{
	float microfacet = 0.76f;

	float halfVector = normalize(light + view);

	// 内積の計算
	float dotNH = saturate(dot(normal, halfVector));
	float dotVH = saturate(dot(view, halfVector));

	float dotNL = saturate(dot(normal, light));
	float dotNV = saturate(dot(normal, view));

	// D項
	float d = Beckmann(microfacet, dotNH);

	// F項
	float f = Fresnel(metallic, dotVH);

	// G項
	float g = GeometricDamping(dotNH, dotNV, dotVH, dotNL);

	// m項
	float m = dotNV * dotNL * PI;
	return max(f * d * g / m, 0.0f);

}

// ピクセルシェーダー
float4 main(PSInput input) : SV_TARGET
{
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
		float3 diffuse = diffuseColor.xyz * lambertDiffuse * diffuseFresnel;

		// BRDF鏡面反射
		float specular = CookTrranceSpecular(lightVector, toEye, input.normal, testSmooth) * directionalLight[i].lightColor.xyz;

		// 金属度が高ければスぺキュラ反射には色が付く、低ければ白
		specular *= lerp(float3(1.0f, 1.0f, 1.0f), float3(0.0f, 0.0f, 0.0f), testMetallic);
		// 拡散反射と鏡面反射の合成
		light += diffuse * (1.0f - testSmooth) + specular;
	}

	return float4(light.xyz, 1.0f);
}