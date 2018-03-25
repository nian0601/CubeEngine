#pragma once
#include <d3d11.h>

struct ID3D11Buffer;

class CE_GPUContext;
class CE_GPUBuffer
{
public:
	CE_GPUBuffer(const CE_GPUContext& aGPUContext);
	~CE_GPUBuffer();

	void InitVertexBuffer(void* someVertices, int aVertexCount, unsigned int aVertexSize);
	void InitIndexBuffer(void* someIndices, int aIndexCount);

	void InitObjectData(unsigned int aSize, int aBufferIndex);
	void* GetObjectData();

	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY aTopology);

	void Render();

private:
	const CE_GPUContext& myGPUContext;

	ID3D11Buffer* myVertexBuffer;
	int myVertexCount;
	unsigned int myVertexSize;

	ID3D11Buffer* myIndexBuffer;
	int myIndexCount;

	ID3D11Buffer* myObjectDataBuffer;
	D3D11_MAPPED_SUBRESOURCE myObjectDataResource;
	int myObjectDataBufferIndex;

	D3D11_PRIMITIVE_TOPOLOGY myTopology;
};