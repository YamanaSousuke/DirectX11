struct PSINPUT
{
	float4 position : SV_POSITION;
	float4 uv : TEXCOORD;
};


Texture2D offDiffuseTexture;
SamplerState offDiffuseTextureSampler;

float4 main(PSINPUT input) : SV_TARGET
{
	return offDiffuseTexture.Sample(offDiffuseTextureSampler, input.uv);
}