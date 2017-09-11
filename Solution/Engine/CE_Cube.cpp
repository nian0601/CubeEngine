#include "stdafx.h"
#include "CE_Cube.h"
#include <d3d11.h>


CE_Cube::CE_Cube()
	: myVertexBuffer(nullptr)
	, myIndexBuffer(nullptr)
{
}


CE_Cube::~CE_Cube()
{
	CE_SAFE_RELEASE(myIndexBuffer);
	CE_SAFE_RELEASE(myVertexBuffer);
}

void CE_Cube::Init(ID3D11Device* aDevice)
{
#pragma region Vertices
	myVertexCount = 24;
	VertexType* vertices = new VertexType[myVertexCount];

	float size = 1.f;
	float halfWidth = size / 2.f;
	float halfHeight = size / 2.f;
	float halfDepth = size / 2.f;
	CE_Vector4f aColour(1.f, 1.f, 1.f, 1.f);

	//0 - 3 (Top)
	vertices[0] = { { -halfWidth, halfHeight, -halfDepth }, aColour };
	vertices[1] = { { halfWidth, halfHeight, -halfDepth }, aColour };
	vertices[2] = { { halfWidth, halfHeight, halfDepth }, aColour };
	vertices[3] = { { -halfWidth, halfHeight, halfDepth }, aColour };

	//4 - 7 (Bottom)
	vertices[4] = { { -halfWidth, -halfHeight, -halfDepth }, aColour };
	vertices[5] = { { halfWidth, -halfHeight, -halfDepth }, aColour };
	vertices[6] = { { halfWidth, -halfHeight, halfDepth }, aColour };
	vertices[7] = { { -halfWidth, -halfHeight, halfDepth }, aColour };

	//8 - 11 (Left)
	vertices[8] = { { -halfWidth, -halfHeight, halfDepth }, aColour };
	vertices[9] = { { -halfWidth, -halfHeight, -halfDepth }, aColour };
	vertices[10] = { { -halfWidth, halfHeight, -halfDepth }, aColour };
	vertices[11] = { { -halfWidth, halfHeight, halfDepth }, aColour };

	//12 - 15 (Right)
	vertices[12] = { { halfWidth, -halfHeight, halfDepth }, aColour };
	vertices[13] = { { halfWidth, -halfHeight, -halfDepth }, aColour };
	vertices[14] = { { halfWidth, halfHeight, -halfDepth }, aColour };
	vertices[15] = { { halfWidth, halfHeight, halfDepth }, aColour };

	//16 - 19 (Front)
	vertices[16] = { { -halfWidth, -halfHeight, -halfDepth }, aColour };
	vertices[17] = { { halfWidth, -halfHeight, -halfDepth }, aColour };
	vertices[18] = { { halfWidth, halfHeight, -halfDepth }, aColour };
	vertices[19] = { { -halfWidth, halfHeight, -halfDepth }, aColour };

	//20 - 23 (Back)
	vertices[20] = { { -halfWidth, -halfHeight, halfDepth }, aColour };
	vertices[21] = { { halfWidth, -halfHeight, halfDepth }, aColour };
	vertices[22] = { { halfWidth, halfHeight, halfDepth }, aColour };
	vertices[23] = { { -halfWidth, halfHeight, halfDepth }, aColour };
#pragma endregion

#pragma region Indices
	myIndexCount = 36;
	unsigned long* indices = new unsigned long[myIndexCount];
	//Top
	indices[0] = 3;
	indices[1] = 1;
	indices[2] = 0;

	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	//Bottom
	indices[6] = 6;
	indices[7] = 4;
	indices[8] = 5;

	indices[9] = 7;
	indices[10] = 4;
	indices[11] = 6;

	//Left
	indices[12] = 11;
	indices[13] = 9;
	indices[14] = 8;

	indices[15] = 10;
	indices[16] = 9;
	indices[17] = 11;

	//Right
	indices[18] = 14;
	indices[19] = 12;
	indices[20] = 13;

	indices[21] = 15;
	indices[22] = 12;
	indices[23] = 14;

	//Front
	indices[24] = 19;
	indices[25] = 17;
	indices[26] = 16;

	indices[27] = 18;
	indices[28] = 17;
	indices[29] = 19;

	//Back
	indices[30] = 22;
	indices[31] = 20;
	indices[32] = 21;

	indices[33] = 23;
	indices[34] = 20;
	indices[35] = 22;
#pragma endregion


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

void CE_Cube::Render(ID3D11DeviceContext* aContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	aContext->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	aContext->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	aContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int CE_Cube::GetIndexCount()
{
	return myIndexCount;
}