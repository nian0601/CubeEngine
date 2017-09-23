#include "stdafx.h"
#include "CE_Model.h"
#include <d3d11.h>
#include "CE_GPUContext.h"


CE_Model::CE_Model()
	: myVertexBuffer(nullptr)
	, myIndexBuffer(nullptr)
	, myObjectDataBuffer(nullptr)
{
}


CE_Model::~CE_Model()
{
	CE_SAFE_RELEASE(myObjectDataBuffer);
	CE_SAFE_RELEASE(myIndexBuffer);
	CE_SAFE_RELEASE(myVertexBuffer);
}

void CE_Model::InitTriangle(const CE_GPUContext& aGPUContext)
{
	myVertexCount = 3;
	VertexType* vertices = new VertexType[myVertexCount];
	vertices[0].myPosition = CE_Vector3f(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].myColor = CE_Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].myPosition = CE_Vector3f(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].myColor = CE_Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].myPosition = CE_Vector3f(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].myColor = CE_Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

	// Load the index array with data.
	myIndexCount = 3;
	UINT* indices = new UINT[myIndexCount];
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	InitBuffers(aGPUContext, vertices, indices);

	CE_SAFE_DELETE_ARRAY(vertices);
	CE_SAFE_DELETE_ARRAY(indices);
}

void CE_Model::InitCube(const CE_GPUContext& aGPUContext)
{
#pragma region Vertices
	myVertexCount = 24;
	VertexType* vertices = new VertexType[myVertexCount];

	float size = 1.f;
	float halfWidth = size / 2.f;
	float halfHeight = size / 2.f;
	float halfDepth = size / 2.f;
	CE_Vector4f aColour(1.f, 1.f, 1.f, 1.f);

	CE_Vector3f up(0.f, 1.f, 0.f);
	CE_Vector3f down(0.f, -1.f, 0.f);

	CE_Vector3f left(-1.f, 0.f, 0.f);
	CE_Vector3f right(1.f, 0.f, 0.f);

	CE_Vector3f forward(0.f, 0.f, 1.f);
	CE_Vector3f back(0.f, 0.f, -1.f);

	//0 - 3 (Top)
	vertices[0] = { { -halfWidth, halfHeight, -halfDepth }, up, aColour };
	vertices[1] = { { halfWidth, halfHeight, -halfDepth }, up, aColour };
	vertices[2] = { { halfWidth, halfHeight, halfDepth }, up, aColour };
	vertices[3] = { { -halfWidth, halfHeight, halfDepth }, up, aColour };

	//4 - 7 (Bottom)
	vertices[4] = { { -halfWidth, -halfHeight, -halfDepth }, down, aColour };
	vertices[5] = { { halfWidth, -halfHeight, -halfDepth }, down, aColour };
	vertices[6] = { { halfWidth, -halfHeight, halfDepth }, down, aColour };
	vertices[7] = { { -halfWidth, -halfHeight, halfDepth }, down, aColour };

	//8 - 11 (Left)
	vertices[8] = { { -halfWidth, -halfHeight, halfDepth }, left, aColour };
	vertices[9] = { { -halfWidth, -halfHeight, -halfDepth }, left, aColour };
	vertices[10] = { { -halfWidth, halfHeight, -halfDepth }, left, aColour };
	vertices[11] = { { -halfWidth, halfHeight, halfDepth }, left, aColour };

	//12 - 15 (Right)
	vertices[12] = { { halfWidth, -halfHeight, halfDepth }, right, aColour };
	vertices[13] = { { halfWidth, -halfHeight, -halfDepth }, right, aColour };
	vertices[14] = { { halfWidth, halfHeight, -halfDepth }, right, aColour };
	vertices[15] = { { halfWidth, halfHeight, halfDepth }, right, aColour };

	//16 - 19 (Front)
	vertices[16] = { { -halfWidth, -halfHeight, -halfDepth }, forward, aColour };
	vertices[17] = { { halfWidth, -halfHeight, -halfDepth }, forward, aColour };
	vertices[18] = { { halfWidth, halfHeight, -halfDepth }, forward, aColour };
	vertices[19] = { { -halfWidth, halfHeight, -halfDepth }, forward, aColour };

	//20 - 23 (Back)
	vertices[20] = { { -halfWidth, -halfHeight, halfDepth }, back, aColour };
	vertices[21] = { { halfWidth, -halfHeight, halfDepth }, back, aColour };
	vertices[22] = { { halfWidth, halfHeight, halfDepth }, back, aColour };
	vertices[23] = { { -halfWidth, halfHeight, halfDepth }, back, aColour };
#pragma endregion

#pragma region Indices
	myIndexCount = 36;
	UINT* indices = new UINT[myIndexCount];
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

	InitBuffers(aGPUContext, vertices, indices);

	CE_SAFE_DELETE_ARRAY(vertices);
	CE_SAFE_DELETE_ARRAY(indices);
}

void CE_Model::Render(const CE_GPUContext& aGPUContext)
{
	ID3D11DeviceContext* context = aGPUContext.GetContext();

	//Update constant-buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	context->Map(myObjectDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	ObjectData* dataPtr = (ObjectData*)mappedResource.pData;
	dataPtr->myWorld = myOrientation;
	dataPtr->myColor = myColor;
	dataPtr->myScale.x = myScale.x;
	dataPtr->myScale.y = myScale.y;
	dataPtr->myScale.z = myScale.z;
	dataPtr->myScale.w = 1.f;

	context->Unmap(myObjectDataBuffer, 0);

	context->VSSetConstantBuffers(1, 1, &myObjectDataBuffer);


	// Set Vertex/Indexbuffers and Topology
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	aGPUContext.GetContext()->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	aGPUContext.GetContext()->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	aGPUContext.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// Actually render
	context->DrawIndexed(GetIndexCount(), 0, 0);
}

void CE_Model::SetOrientation(const CE_Matrix44f& anOrientation)
{
	myOrientation = anOrientation;
}

void CE_Model::SetPosition(const CE_Vector3f& aPosition)
{
	myOrientation.SetPos(aPosition);
}

void CE_Model::Rotate(const CE_Matrix44f& aRotation)
{
	CE_Vector3f pos = myOrientation.GetPos();
	myOrientation.SetPos(CE_Vector3f());
	myOrientation = myOrientation * aRotation;
	myOrientation.SetPos(pos);
}

void CE_Model::SetScale(const CE_Vector3f& aScale)
{
	myScale = aScale;
}

void CE_Model::SetColor(const CE_Vector4f& aColor)
{
	myColor = aColor;
}

void CE_Model::InitBuffers(const CE_GPUContext& aGPUContext, void* aVertexData, void* aIndexData)
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
