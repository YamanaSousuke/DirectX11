#include "BasicShader.hlsli"

static const float roughness = 0.5f;
static const float microfacet = 0.3f;
static const float maxEnergy = 1.5f;

// フレネル反射を考慮した拡散反射率の計算
float CalcDiffuse(float3 normal, float3 light, float3 view)
{
	float halfVector = (light + view);
	float dotLH = saturate(dot(light, halfVector));

	// 1を超えないようにmaxEnergyで正規化をする
	float energyBias = lerp(0.0f, 0.5f, roughness);
	float energyFactor = lerp(1.0f, 1.0f / maxEnergy, roughness);

	// 光が平行に入射したときの拡散反射率
	float f90 = energyBias + 2.0f * dotLH * dotLH * roughness;
	// 法線と光源に向かうベクトルの拡散反射率
	float dotNL = saturate(dot(normal, light));
	float lightScatter = (1 + (f90 - 1) * pow(1 - dotNL, 5));
	// 法線と視線に向かうベクトルの拡散反射率
	float dotNV = saturate(dot(normal, view));
	float viewScatter = (1 + (f90 - 1) * pow(1 - dotNV, 5));
	return (lightScatter * viewScatter * energyFactor / PI);
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
	float m = dotNV * dotNH * PI;
	return max(f * d * g / m, 0.0f);
}

// ピクセルシェーダー
float4 main(PSInput input) : SV_TARGET
{
	// テクスチャーのサンプリング
	float4 albedoColor = diffuseTexture.Sample(diffuseSampler, input.texCoord);
	float3 specularColor = albedoColor.xyz;

	// 視線ベクトルと視線までの距離
	float3 toEye = normalize(eyePosition - input.worldPosition);
	float distanceToEye = distance(eyePosition, input.worldPosition);

	// マテリアルの強さの取得
	float ambientFactor = material.materialAmbient[3];
	float diffuseFactor = material.materialDiffuse[3];
	
	float3 light = 0.0f;
	float4 ambientColor = 0.0f;
	[unroll]
	for (int i = 0; i < numDirectionalLight; i++) {
		// 環境光
		ambientColor += material.materialAmbient * ambientFactor * directionalLight[i].lightAmbient;

		// 拡散反射
		float3 lightVector = normalize(-directionalLight[i].lightDirection.xyz);
		float diffuseFresnel = CalcDiffuse(input.normal, lightVector, toEye);

		// 正規化ランバート反射
		float dotNL = saturate(dot(lightVector.xyz, input.normal));
		float3 lambertDiffuse = directionalLight[i].lightDiffuse.xyz * dotNL / PI;
		float3 diffuse = material.materialDiffuse.xyz * lambertDiffuse * diffuseFresnel * albedoColor * diffuseFactor;

		// 鏡面反射
		float3 specular = CookTrranceSpecular(lightVector, toEye, input.normal, material.smooth) * directionalLight[i].lightDiffuse.xyz;

		// 金属度が高ければスぺキュラ反射には色が付く、低ければ白
		specular *= lerp(float3(1.0f, 1.0f, 1.0f), specularColor, material.metallic);
		// 拡散反射と鏡面反射の合成
		light += diffuse * (1.0f - material.smooth) + specular;
	}
	light += ambientColor * albedoColor;

	// フォグ
	[flatten]
	if (fogEnable) {
		float fogLerp = saturate((distanceToEye - fogStart) / fogRange);
		light = lerp(light, fogColor, fogLerp);
	}
	return float4(light, 1.0f);
}