#include "stdafx.h"
#include "CE_LineRenderObject.h"
#include <d3d11.h>
#include "CE_GPUContext.h"


CE_LineRenderObject::CE_LineRenderObject()
	: myVertexBuffer(nullptr)
	, myIndexBuffer(nullptr)
	, myVertexCount(0)
	, myIndexCount(0)
	, myVertices(nullptr)
	, myIndices(nullptr)
{
}


CE_LineRenderObject::~CE_LineRenderObject()
{
	CE_SAFE_RELEASE(myIndexBuffer);
	CE_SAFE_RELEASE(myVertexBuffer);
}

void CE_LineRenderObject::SetLines(const CE_GrowingArray<CE_Line>& someLines, const CE_GPUContext& aGPUContext)
{
	if (myVertexCount < someLines.Size() * 2)
	{
		myVertexCount = someLines.Size() * 2;
		myIndexCount = myVertexCount;

		CE_SAFE_DELETE_ARRAY(myVertices);
		CE_SAFE_DELETE_ARRAY(myIndices);

		myVertices = new VertexType[myVertexCount];
		myIndices = new UINT[myIndexCount];
	}


	int realIndex = 0;
	for (int i = 0; i < someLines.Size(); ++i)
	{
		myVertices[realIndex].myPosition = someLines[i].myStart;
		myVertices[realIndex].myColor = someLines[i].myStartColor;

		myVertices[realIndex + 1].myPosition = someLines[i].myEnd;
		myVertices[realIndex + 1].myColor = someLines[i].myEndColor;

		myIndices[realIndex] = realIndex;  // Bottom left.
		myIndices[realIndex + 1] = realIndex + 1;  // Top middle.

		realIndex += 2;
	}

	InitVertexAndIndexBuffers(aGPUContext, myVertices, myIndices);
}

void CE_LineRenderObject::Render(const CE_GPUContext& aGPUContext)
{
	// Set Vertex/Indexbuffers and Topology
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	aGPUContext.GetContext()->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	aGPUContext.GetContext()->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	aGPUContext.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Actually render
	ID3D11DeviceContext* context = aGPUContext.GetContext();
	context->DrawIndexed(myIndexCount, 0, 0);
}

void CE_LineRenderObject::InitVertexAndIndexBuffers(const CE_GPUContext& aGPUContext, void* aVertexData, void* aIndexData)
{
	CE_SAFE_RELEASE(myIndexBuffer);
	CE_SAFE_RELEASE(myVertexBuffer);

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
}