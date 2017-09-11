cbuffer MatrixBuffer
{
	Matrix WorldMatrix;
	Matrix ViewProjectionMatrix;
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

	Matrix mat = mul(WorldMatrix, ViewProjectionMatrix);
	output.Position = mul(aInput.Position, mat);
	output.Color = aInput.Color;
	return output;
};