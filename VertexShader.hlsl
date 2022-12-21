// �萔�o�b�t�@�[
cbuffer ConstantBuffer
{
	matrix world;					// ���[���h�s��
	matrix view;					// �r���[�s��
	matrix projection;				// �v���W�F�N�V�����s��
	matrix worldViewProjection;		// WVP�s��
};

struct VSInput
{
	float4 position : POSITION;
};


struct VSOutput
{
	float4 position : SV_POSITION;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.position = mul(input.position, worldViewProjection);
	return output;
}