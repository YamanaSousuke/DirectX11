
struct VSINPUT
{
	float4 position : POSITION;
	float4 uv : TEXCOORD;
};

struct VSOUTPUT
{
	float4 position : SV_POSITION;
	float4 uv : TEXCOORD;
};

VSOUTPUT main(VSINPUT input)
{
	VSOUTPUT output;
	output.position = input.position;
	output.uv = input.uv;
	return output;
}