cbuffer MatrixBuffer
{
	Matrix WorldMatrix;
	Matrix ViewMatrix;
	Matrix ProjectionMatrix;
};

struct PosColorVertex
{
	float4 Position : POSITION;
	float4 Color : COLOR;
};

struct PosColorPixel
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};


PosColorPixel ColorVertexShader(PosColorVertex aInput)
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