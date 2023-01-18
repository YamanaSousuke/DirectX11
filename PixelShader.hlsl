#include "BasicShader.hlsli"

Texture2D diffuseTexture;
SamplerState diffuseTextureSampler;

// �G���g���[�|�C���g
float4 main(PSInput input) : SV_TARGET
{
	float4 texel = diffuseTexture.Sample(diffuseTextureSampler, input.texCoord);
	return float4(texel.xyz, input.color.a);
}