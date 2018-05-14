#pragma once

#include "CE_DebugPrimitives.h"

struct ID3D11Buffer;

class CE_GPUContext;

class CE_LineRenderObject
{
public:
	CE_LineRenderObject();
	~CE_LineRenderObject();

	void SetLine(const CE_Line& aLine, const CE_GPUContext& aGPUContext);
	void SetLines(const CE_GrowingArray<CE_Line>& someLines, const CE_GPUContext& aGPUContext);

	void Render(const CE_GPUContext& aGPUContext);

private:
	void InitVertexAndIndexBuffers(const CE_GPUContext& aGPUContext, void* aVertexData, void* aIndexData, int aVertexCount);

	struct VertexType
	{
		CE_Vector4f myPosition;
		CE_Vector4f myColor;
	};

	VertexType* myVertices;
	unsigned int* myIndices;

	ID3D11Buffer* myVertexBuffer;
	int myVertexCount;

	ID3D11Buffer* myIndexBuffer;
	int myIndexCount;

	int myRealVertexCount;
	int myRealIndexCount;
};

