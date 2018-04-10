#include "stdafx.h"
#include "CE_GPUBuffer.h"
#include <d3d11.h>
#include "CE_GPUContext.h"


CE_GPUBuffer::CE_GPUBuffer(const CE_GPUContext& aGPUContext)
	: myGPUContext(aGPUContext)
	, myVertexBuffer(nullptr)
	, myIndexBuffer(nullptr)
{
}


CE_GPUBuffer::~CE_GPUBuffer()
{
	CE_SAFE_RELEASE(myVertexBuffer);
	CE_SAFE_RELEASE(myIndexBuffer);
}

void CE_GPUBuffer::InitVertexBuffer(void* someVertices, int aVertexCount, unsigned int aVertexSize)
{
	CE_SAFE_RELEASE(myVertexBuffer);
	myVertexSize = aVertexSize;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = aVertexSize * aVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = someVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT result = myGPUContext.GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer);
	CE_ASSERT(SUCCEEDED(result), "Failed to create VertexBuffer");
}

void CE_GPUBuffer::InitIndexBuffer(void* someIndices, int aIndexCount)
{
	CE_SAFE_RELEASE(myIndexBuffer);

	myIndexCount = aIndexCount;

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * aIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = someIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	HRESULT result = myGPUContext.GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &myIndexBuffer);
	CE_ASSERT(SUCCEEDED(result), "Failed to create IndexBuffer");
}

void CE_GPUBuffer::Render()
{
	ID3D11DeviceContext* context = myGPUContext.GetContext();
	unsigned int stride = myVertexSize;
	unsigned int offset = 0;

	if (myObjectDataBuffer)
	{
		context->Unmap(myObjectDataBuffer, 0);
		context->VSSetConstantBuffers(myObjectDataBufferIndex, 1, &myObjectDataBuffer);
		context->PSSetConstantBuffers(myObjectDataBufferIndex, 1, &myObjectDataBuffer);
	}

	context ->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	context ->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context ->IASetPrimitiveTopology(myTopology);

	// Actually render
	context->DrawIndexed(myIndexCount, 0, 0);
}

void CE_GPUBuffer::InitObjectData(unsigned int aSize, int aBufferIndex)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = aSize;
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	ID3D11Device* device = myGPUContext.GetDevice();
	HRESULT result = device->CreateBuffer(&matrixBufferDesc, NULL, &myObjectDataBuffer);
	CE_ASSERT(FAILED(result) == false, "Failed to create GlobalDataBuffer");

	myObjectDataBufferIndex = aBufferIndex;
}

void* CE_GPUBuffer::GetObjectData()
{
	CE_ASSERT(myObjectDataBuffer != nullptr, "No GlobalData created for this shader");
	ID3D11DeviceContext* context = myGPUContext.GetContext();
	HRESULT result = context->Map(myObjectDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &myObjectDataResource);
	CE_ASSERT(SUCCEEDED(result), "Mapping failed");
	return myObjectDataResource.pData;
}

void CE_GPUBuffer::SetTopology(D3D11_PRIMITIVE_TOPOLOGY aTopology)
{
	myTopology = aTopology;
}
