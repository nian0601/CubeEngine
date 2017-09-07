#include "stdafx.h"
#include "CE_Model.h"
#include <d3d11.h>


CE_Model::CE_Model()
	: myVertexBuffer(nullptr)
	, myIndexBuffer(nullptr)
{
}


CE_Model::~CE_Model()
{
}

void CE_Model::Init(ID3D11Device* aDevice)
{
	myVertexCount = 3;
	VertexType* vertices = new VertexType[myVertexCount];

	myIndexCount = 3;
	unsigned long* indices = new unsigned long[myIndexCount];

	vertices[0].myPosition = CE_Vector3f(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].myColor = CE_Vector4f(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].myPosition = CE_Vector3f(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].myColor = CE_Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].myPosition = CE_Vector3f(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].myColor = CE_Vector4f(0.0f, 0.0f, 1.0f, 1.0f);

	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.


	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * myVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT result;
	result = aDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer);
	if (FAILED(result))
		return;

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * myIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = aDevice->CreateBuffer(&indexBufferDesc, &indexData, &myIndexBuffer);
	if (FAILED(result))
		return;

	CE_SAFE_DELETE_ARRAY(vertices);
	CE_SAFE_DELETE_ARRAY(indices);
}

void CE_Model::Shutdown()
{
	CE_SAFE_RELEASE(myIndexBuffer);
	CE_SAFE_RELEASE(myVertexBuffer);
}

void CE_Model::Render(ID3D11DeviceContext* aContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	aContext->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	aContext->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	aContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int CE_Model::GetIndexCount()
{
	return myIndexCount;
}
