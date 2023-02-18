#include "BasicShader.hlsli"

// �G���g���[�|�C���g
VSOutput main(VSInput input)
{
	VSOutput output;
	output.position = input.position;
	output.worldPosition = input.position;
	output.color = input.color;
	output.normal = input.normal;
	output.texCoord = input.texCoord;
	return output;
}
