#pragma once

struct ID3D11Buffer;

class CE_GPUContext;
class CE_GPUBuffer
{
public:
	CE_GPUBuffer();
	~CE_GPUBuffer();

	ID3D11Buffer* myBuffer;

protected:
	void InternalInitStatic(void* someData, int aElementCount, int aElementSize, int aBindType);
};

class CE_GPUVertexBuffer : public CE_GPUBuffer
{
public:
	void InitStatic(void* someData, int aElementCount, int aElementSize);
	void InitDynamic();

	int myVertexSize;
	int myVertexCount;
};

class CE_GPUIndexBuffer : public CE_GPUBuffer
{
public:
	void InitStatic(void* someData, int aElementCount, int aElementSize);

	int myIndexCount;
};

class CE_ConstantBuffer : public CE_GPUBuffer
{
public:
	void Init(int aBufferIndex, int aDataSize);
	void SetData(void* someData, int aDataSize);

	void SendToGPU();

	int myBufferIndex;
	int myDataSize;
};