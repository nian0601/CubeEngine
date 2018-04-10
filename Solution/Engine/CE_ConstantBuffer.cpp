#include "stdafx.h"
#include "CE_ConstantBuffer.h"
#include "CE_GPUContext.h"

#include <d3d11.h>

CE_ConstantBuffer::CE_ConstantBuffer(const CE_GPUContext& aGPUContext)
	: myGPUContext(aGPUContext)
	, myBuffer(nullptr)
	, myBufferIndex(-1)
	, myBufferSize(static_cast<unsigned int>(-1))
{
}


CE_ConstantBuffer::~CE_ConstantBuffer()
{
	CE_SAFE_RELEASE(myBuffer);
}

void CE_ConstantBuffer::Init(unsigned int aBufferSize, int aBufferIndex)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = aBufferSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	ID3D11Device* device = myGPUContext.GetDevice();
	HRESULT result = device->CreateBuffer(&bufferDesc, NULL, &myBuffer);
	CE_ASSERT(FAILED(result) == false, "Failed to create GlobalDataBuffer");

	myBufferIndex = aBufferIndex;
	myBufferSize = aBufferSize;
}

void CE_ConstantBuffer::Update(void* someData, unsigned int someDataSize)
{
	CE_ASSERT(myBufferSize == someDataSize, "Missmatching datasize")
	CE_ASSERT(myBuffer != nullptr, "ConstantBuffer is not initialized");

	ID3D11DeviceContext* context = myGPUContext.GetContext();

	D3D11_MAPPED_SUBRESOURCE dataResource;
	HRESULT result = context->Map(myBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataResource);
	CE_ASSERT(SUCCEEDED(result), "Mapping failed");
	CE_MEM_COPY(dataResource.pData, someData, someDataSize);

	context->Unmap(myBuffer, 0);
}

void CE_ConstantBuffer::SendToGPU()
{
	CE_ASSERT(myBuffer != nullptr, "ConstantBuffer is not initialized");

	ID3D11DeviceContext* context = myGPUContext.GetContext();
	context->VSSetConstantBuffers(myBufferIndex, 1, &myBuffer);
	context->PSSetConstantBuffers(myBufferIndex, 1, &myBuffer);
}
