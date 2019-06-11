#include "stdafx.h"

#include "CE_GPUBuffer.h"
#include "CE_GPUContext.h"

#include <d3d11.h>

CE_GPUBuffer::CE_GPUBuffer()
	: myBuffer(nullptr)
{
}

CE_GPUBuffer::~CE_GPUBuffer()
{
	CE_SAFE_RELEASE(myBuffer);
}

void CE_GPUBuffer::InternalInitStatic(void* someData, int aElementCount, int aElementSize, int aBindType)
{
	CE_ASSERT(myBuffer == nullptr, "Buffer allready initialized");

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = aElementSize * aElementCount;
	bufferDesc.BindFlags = aBindType;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = someData;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	ID3D11Device* device = CE_Engine::GetGPUContext().GetDevice();
	HRESULT result = device->CreateBuffer(&bufferDesc, &initData, &myBuffer);
	CE_ASSERT(SUCCEEDED(result), "Failed to create Buffer");
}

//////////////////////////////////////////////////////////////////////////

void CE_GPUVertexBuffer::InitStatic(void* someData, int aElementCount, int aElementSize)
{
	myVertexCount = aElementCount;
	myVertexSize = aElementSize;

	InternalInitStatic(someData, aElementCount, aElementSize, D3D11_BIND_VERTEX_BUFFER);
}

void CE_GPUVertexBuffer::InitDynamic(int aElementCount, int aElementSize)
{
	myVertexCount = aElementCount;
	myVertexSize = aElementSize;

	CE_ASSERT(myBuffer == nullptr, "Buffer allready initialized");

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = myVertexCount * myVertexSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	ID3D11Device* device = CE_Engine::GetGPUContext().GetDevice();
	HRESULT result = device->CreateBuffer(&bufferDesc, NULL, &myBuffer);
	CE_ASSERT(FAILED(result) == false, "Failed to create Buffer");
}

void CE_GPUVertexBuffer::UpdateDynamic(void* someData, int aElementCount, int aElementSize)
{
	CE_ASSERT(myBuffer != nullptr, "Buffer not initialized");
	CE_ASSERT(myVertexCount == aElementCount && myVertexSize == aElementSize, "Missmatching sizes!");

	ID3D11DeviceContext* context = CE_Engine::GetGPUContext().GetContext();

	D3D11_MAPPED_SUBRESOURCE bufferData;
	HRESULT result = context->Map(myBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	CE_ASSERT(SUCCEEDED(result), "Mapping failed");

	memcpy(bufferData.pData, someData, aElementCount * aElementSize);

	context->Unmap(myBuffer, 0);
}

//////////////////////////////////////////////////////////////////////////

void CE_GPUIndexBuffer::InitStatic(void* someData, int aElementCount, int aElementSize)
{
	myIndexCount = aElementCount;

	InternalInitStatic(someData, aElementCount, aElementSize, D3D11_BIND_INDEX_BUFFER);
}

//////////////////////////////////////////////////////////////////////////

void CE_ConstantBuffer::Init(int aBufferIndex, int aDataSize)
{
	CE_ASSERT(myBuffer == nullptr, "Buffer allready initialized");

	myBufferIndex = aBufferIndex;
	myDataSize = aDataSize;

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = aDataSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	ID3D11Device* device = CE_Engine::GetGPUContext().GetDevice();
	HRESULT result = device->CreateBuffer(&bufferDesc, NULL, &myBuffer);
	CE_ASSERT(FAILED(result) == false, "Failed to create GlobalDataBuffer");
}

void CE_ConstantBuffer::SetData(void* someData, int aDataSize)
{
	CE_ASSERT(myBuffer != nullptr, "Buffer not initialized");
	CE_ASSERT(myDataSize == aDataSize, "Missmatching sizes!");

	ID3D11DeviceContext* context = CE_Engine::GetGPUContext().GetContext();

	D3D11_MAPPED_SUBRESOURCE bufferData;
	HRESULT result = context->Map(myBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	CE_ASSERT(SUCCEEDED(result), "Mapping failed");

	memcpy(bufferData.pData, someData, aDataSize);

	context->Unmap(myBuffer, 0);
}

void CE_ConstantBuffer::SendToGPU()
{
	CE_ASSERT(myBuffer != nullptr, "ConstantBuffer is not initialized");
	ID3D11DeviceContext* context = CE_Engine::GetGPUContext().GetContext();
	context->VSSetConstantBuffers(myBufferIndex, 1, &myBuffer);
	context->PSSetConstantBuffers(myBufferIndex, 1, &myBuffer);
}
