#include "BasicShader.hlsli"

VSOutput main(VSInput input)
{
	VSOutput output;
	output.position = input.position;
	output.normal = input.normal;
	return output;
}
