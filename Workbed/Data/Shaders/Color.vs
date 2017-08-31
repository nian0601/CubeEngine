cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

struct PosColorVertex
{
	float4 Position : POSITION;
	float4 Color : COLOR;
};

stuct PosColorPixel
{
	float4 Position : SV_POSITION,
	float4 Coolor : COLOR;
};


PosColorPixel VertexShader(PosColorVertex aInput)
{
	PosColorPixel output;
	output.Position = aInput.Position;
	output.Position.w = 1.f;

	output.Position = mul(output.Position, WorldMatrix);
	output.Position = mul(output.Position, ViewMatrix);
	output.Position = mul(output.Position, ProjectionMatrix);

	output.Color = aInput.Color;
	return output;
};