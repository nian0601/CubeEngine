#pragma once

struct ID3D11Buffer;

class CE_ConstantBuffer
{
public:
	CE_ConstantBuffer(const CE_GPUContext& aGPUContext);
	~CE_ConstantBuffer();

	void Init(unsigned int aBufferSize, int aBufferIndex);
	void Update(void* someData, unsigned int someDataSize);

	void SendToGPU();

private:
	const CE_GPUContext& myGPUContext;

	ID3D11Buffer* myBuffer;
	unsigned int myBufferSize;
	int myBufferIndex;
};

