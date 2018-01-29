#include "stdafx.h"

#include "CE_GPUContext.h"
#include "CE_Sprite.h"

#include <d3d11.h>

CE_Sprite::CE_Sprite()
	: myVertexBuffer(nullptr)
	, myIndexBuffer(nullptr)
	, myObjectDataBuffer(nullptr)
{
}


CE_Sprite::~CE_Sprite()
{
	CE_SAFE_RELEASE(myObjectDataBuffer);
	CE_SAFE_RELEASE(myIndexBuffer);
	CE_SAFE_RELEASE(myVertexBuffer);
}

void CE_Sprite::Init(const CE_GPUContext& aGPUContext)
{
	myVertexCount = 4;
	VertexType* vertices = new VertexType[myVertexCount];
	vertices[0].myPosition = CE_Vector3f(-1.0f, -1.0f, 0.0f); //topleft
	vertices[1].myPosition = CE_Vector3f(1.0f, -1.0f, 0.0f); //topright
	vertices[2].myPosition = CE_Vector3f(-1.0f, 1.0f, 0.0f); //bottomleft
	vertices[3].myPosition = CE_Vector3f(1.0f, 1.0f, 0.0f); //bottomright

	// Load the index array with data.
	myIndexCount = 6;
	UINT* indices = new UINT[myIndexCount];
	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;

	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;;

	InitBuffers(aGPUContext, vertices, indices);

	CE_SAFE_DELETE_ARRAY(vertices);
	CE_SAFE_DELETE_ARRAY(indices);
}

void CE_Sprite::Render(const CE_GPUContext& aGPUContext)
{
	ID3D11DeviceContext* context = aGPUContext.GetContext();

	//Update constant-buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	context->Map(myObjectDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	CE_Vector2f scaledHotspot = 2.f * myHotspot;

	ObjectData* dataPtr = (ObjectData*)mappedResource.pData;
	dataPtr->myWorld = myOrientation;
	dataPtr->myColor = myColor;
	dataPtr->mySize = mySize * 0.5f;;
	dataPtr->myPosition = myPosition;
	dataPtr->myHotspot = scaledHotspot;

	context->Unmap(myObjectDataBuffer, 0);

	context->VSSetConstantBuffers(1, 1, &myObjectDataBuffer);


	// Set Vertex/Indexbuffers and Topology
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	aGPUContext.GetContext()->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	aGPUContext.GetContext()->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	aGPUContext.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// Actually render
	context->DrawIndexed(myIndexCount, 0, 0);
}

void CE_Sprite::SetPosition(const CE_Vector2f& aPosition)
{
	myPosition = aPosition;
}

void CE_Sprite::SetSize(const CE_Vector2f& aSize)
{
	mySize = aSize;
}

void CE_Sprite::SetHotspot(const CE_Vector2f& aHotspot)
{
	myHotspot = aHotspot;
}

void CE_Sprite::SetColor(const CE_Vector4f& aColor)
{
	myColor = aColor;
}

void CE_Sprite::InitBuffers(const CE_GPUContext& aGPUContext, void* aVertexData, void* aIndexData)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * myVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = aVertexData;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT result;
	result = aGPUContext.GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer);
	if (FAILED(result))
		return;

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * myIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = aIndexData;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = aGPUContext.GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &myIndexBuffer);
	if (FAILED(result))
		return;

	D3D11_BUFFER_DESC objectDataBufferDesc;
	objectDataBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	objectDataBufferDesc.ByteWidth = sizeof(ObjectData);
	objectDataBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	objectDataBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	objectDataBufferDesc.MiscFlags = 0;
	objectDataBufferDesc.StructureByteStride = 0;

	result = aGPUContext.GetDevice()->CreateBuffer(&objectDataBufferDesc, NULL, &myObjectDataBuffer);
}
