// 定数バッファー
cbuffer ConstantBuffer
{
	matrix world;					// ワールド行列
	matrix view;					// ビュー行列
	matrix projection;				// プロジェクション行列
	matrix worldViewProjection;		// WVP行列
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