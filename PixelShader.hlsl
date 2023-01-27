#include "BasicShader.hlsli"

static const int numDirectionalLight = 1;

// マテリアルの表面カラー
static const float4 diffuseColor = float4(1.0f, 1.0f, 0.0f, 1.0f);

float4 main(PSInput input) : SV_TARGET
{
	float3 light = 0.0f;
	for (int i = 0; i < numDirectionalLight; i++)
	{
		float3 lightVector = normalize(directionalLight.lightDirection);
		float lambertDiffuse = saturate(dot(lightVector, input.normal));
		float3 diffuse = diffuseColor.xyz * directionalLight.lightColor.xyz * lambertDiffuse;
		light += diffuse;
	}


	return float4(light.xyz, 1.0f);
}