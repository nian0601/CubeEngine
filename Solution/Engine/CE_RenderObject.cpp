#include "stdafx.h"

#include "CE_GPUContext.h"
#include "CE_GPUBuffer.h"
#include "CE_LightSphereCreator.h"
#include "CE_ModelSphereCreator.h"
#include "CE_ObjLoader.h"
#include "CE_RenderObject.h"
#include "CE_ShaderStructs.h"

#include <d3d11.h>

CE_RenderObject::CE_RenderObject()
	: myVertexBuffer(nullptr)
	, myIndexBuffer(nullptr)
	, myObjectBuffer(nullptr)
{
}

CE_RenderObject::~CE_RenderObject()
{
	CE_SAFE_DELETE(myObjectBuffer);
	CE_SAFE_DELETE(myIndexBuffer);
	CE_SAFE_DELETE(myVertexBuffer);
}

void CE_RenderObject::Init(void* someVertexData, int aVertexCount, int aVertexSize, void* someIndexData, int aIndexCount)
{
	myVertexBuffer = new CE_GPUVertexBuffer();
	myVertexBuffer->InitStatic(someVertexData, aVertexCount, aVertexSize);

	myIndexBuffer = new CE_GPUIndexBuffer();
	myIndexBuffer->InitStatic(someIndexData, aIndexCount, sizeof(UINT));
}

void CE_RenderObject::InitCube()
{
#pragma region Vertices
	const int vertexCount = 24;
	CE_PosNormColor_Vert vertices[vertexCount];

	float size = 1.f;
	float halfWidth = size / 2.f;
	float halfHeight = size / 2.f;
	float halfDepth = size / 2.f;
	CE_Vector4f aColour(1.f, 1.f, 1.f, 1.f);

	CE_Vector4f up(0.f, 1.f, 0.f, 0.f);
	CE_Vector4f down(0.f, -1.f, 0.f, 0.f);
			 
	CE_Vector4f left(-1.f, 0.f, 0.f, 0.f);
	CE_Vector4f right(1.f, 0.f, 0.f, 0.f);
			 
	CE_Vector4f forward(0.f, 0.f, 1.f, 0.f);
	CE_Vector4f back(0.f, 0.f, -1.f, 0.f);

	//0 - 3 (Top)
	vertices[0] = { { -halfWidth, halfHeight, -halfDepth, 1.f }, up, aColour };
	vertices[1] = { { halfWidth, halfHeight, -halfDepth, 1.f }, up, aColour };
	vertices[2] = { { halfWidth, halfHeight, halfDepth, 1.f }, up, aColour };
	vertices[3] = { { -halfWidth, halfHeight, halfDepth, 1.f }, up, aColour };

	//4 - 7 (Bottom)
	vertices[4] = { { -halfWidth, -halfHeight, -halfDepth, 1.f }, down, aColour };
	vertices[5] = { { halfWidth, -halfHeight, -halfDepth, 1.f }, down, aColour };
	vertices[6] = { { halfWidth, -halfHeight, halfDepth, 1.f }, down, aColour };
	vertices[7] = { { -halfWidth, -halfHeight, halfDepth, 1.f }, down, aColour };

	//8 - 11 (Left)
	vertices[8] = { { -halfWidth, -halfHeight, halfDepth, 1.f }, left, aColour };
	vertices[9] = { { -halfWidth, -halfHeight, -halfDepth, 1.f }, left, aColour };
	vertices[10] = { { -halfWidth, halfHeight, -halfDepth, 1.f }, left, aColour };
	vertices[11] = { { -halfWidth, halfHeight, halfDepth, 1.f }, left, aColour };

	//12 - 15 (Right)
	vertices[12] = { { halfWidth, -halfHeight, halfDepth, 1.f }, right, aColour };
	vertices[13] = { { halfWidth, -halfHeight, -halfDepth, 1.f }, right, aColour };
	vertices[14] = { { halfWidth, halfHeight, -halfDepth, 1.f }, right, aColour };
	vertices[15] = { { halfWidth, halfHeight, halfDepth, 1.f }, right, aColour };

	//16 - 19 (Front)
	vertices[16] = { { -halfWidth, -halfHeight, -halfDepth, 1.f }, forward, aColour };
	vertices[17] = { { halfWidth, -halfHeight, -halfDepth, 1.f }, forward, aColour };
	vertices[18] = { { halfWidth, halfHeight, -halfDepth, 1.f }, forward, aColour };
	vertices[19] = { { -halfWidth, halfHeight, -halfDepth, 1.f }, forward, aColour };

	//20 - 23 (Back)
	vertices[20] = { { -halfWidth, -halfHeight, halfDepth, 1.f }, back, aColour };
	vertices[21] = { { halfWidth, -halfHeight, halfDepth, 1.f }, back, aColour };
	vertices[22] = { { halfWidth, halfHeight, halfDepth, 1.f }, back, aColour };
	vertices[23] = { { -halfWidth, halfHeight, halfDepth, 1.f }, back, aColour };
#pragma endregion

#pragma region Indices
	const int indexCount = 36;
	UINT indices[indexCount];
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

	myVertexBuffer = new CE_GPUVertexBuffer();
	myVertexBuffer->InitStatic(vertices, vertexCount, sizeof(CE_PosNormColor_Vert));

	myIndexBuffer = new CE_GPUIndexBuffer();
	myIndexBuffer->InitStatic(indices, indexCount, sizeof(UINT));
}

void CE_RenderObject::InitSprite()
{
	const int vertexCount = 4;
	//CE_Pos_UV_Vert vertices[vertexCount];
	//vertices[0].myPosition = CE_Vector4f(-1.0f, -1.0f, 0.0f, 1.f); //topleft
	//vertices[0].myTexCoord = CE_Vector2f(0.f, 1.f);

	//vertices[1].myPosition = CE_Vector4f(1.0f, -1.0f, 0.0f, 1.f); //topright
	//vertices[1].myTexCoord = CE_Vector2f(1.f, 1.f);

	//vertices[2].myPosition = CE_Vector4f(-1.0f, 1.0f, 0.0f, 1.f); //bottomleft
	//vertices[2].myTexCoord = CE_Vector2f(0.f, 0.f);

	//vertices[3].myPosition = CE_Vector4f(1.0f, 1.0f, 0.0f, 1.f); //bottomright
	//vertices[3].myTexCoord = CE_Vector2f(1.f, 0.f);

	const int indexCount = 6;
	UINT indices[indexCount];
	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;

	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	myVertexBuffer = new CE_GPUVertexBuffer();
	//myVertexBuffer->InitStatic(vertices, vertexCount, sizeof(CE_Pos_UV_Vert));
	myVertexBuffer->InitDynamic(vertexCount, sizeof(CE_Pos_UV_Vert));

	myIndexBuffer = new CE_GPUIndexBuffer();
	myIndexBuffer->InitStatic(indices, indexCount, sizeof(UINT));
}

void CE_RenderObject::InitFullscreenQuad()
{
	const int vertexCount = 4;
	CE_Pos_UV_Vert vertices[vertexCount];
	vertices[0].myPosition = CE_Vector4f(-1.0f, -1.0f, 0.0f, 1.f); //topleft
	vertices[0].myTexCoord = CE_Vector2f(0.f, 1.f);

	vertices[1].myPosition = CE_Vector4f(1.0f, -1.0f, 0.0f, 1.f); //topright
	vertices[1].myTexCoord = CE_Vector2f(1.f, 1.f);

	vertices[2].myPosition = CE_Vector4f(-1.0f, 1.0f, 0.0f, 1.f); //bottomleft
	vertices[2].myTexCoord = CE_Vector2f(0.f, 0.f);

	vertices[3].myPosition = CE_Vector4f(1.0f, 1.0f, 0.0f, 1.f); //bottomright
	vertices[3].myTexCoord = CE_Vector2f(1.f, 0.f);

	const int indexCount = 6;
	UINT indices[indexCount];
	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;

	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	myVertexBuffer = new CE_GPUVertexBuffer();
	myVertexBuffer->InitStatic(vertices, vertexCount, sizeof(CE_Pos_UV_Vert));

	myIndexBuffer = new CE_GPUIndexBuffer();
	myIndexBuffer->InitStatic(indices, indexCount, sizeof(UINT));
}

void CE_RenderObject::InitSphere()
{
	CE_ModelSphereCreator sphereCreator;
	sphereCreator.Create(4);

	myVertexBuffer = new CE_GPUVertexBuffer();
	myVertexBuffer->InitStatic(sphereCreator.myVertices.GetArrayAsPointer(), sphereCreator.myVertices.Size(), sizeof(sphereCreator.myVertices[0]));

	myIndexBuffer = new CE_GPUIndexBuffer();
	myIndexBuffer->InitStatic(sphereCreator.myIndices.GetArrayAsPointer(), sphereCreator.myIndices.Size(), sizeof(sphereCreator.myIndices[0]));
}

void CE_RenderObject::InitLightSphere()
{
	CE_LightSphereCreator sphereCreator;
	sphereCreator.Create(2);

	myVertexBuffer = new CE_GPUVertexBuffer();
	myVertexBuffer->InitStatic(sphereCreator.myVertices.GetArrayAsPointer(), sphereCreator.myVertices.Size(), sizeof(sphereCreator.myVertices[0]));

	myIndexBuffer = new CE_GPUIndexBuffer();
	myIndexBuffer->InitStatic(sphereCreator.myIndices.GetArrayAsPointer(), sphereCreator.myIndices.Size(), sizeof(sphereCreator.myIndices[0]));
}

void CE_RenderObject::Render()
{
	CE_ASSERT(myVertexBuffer, "No VertexBuffer");
	CE_ASSERT(myIndexBuffer, "No IndexBuffer");

	ID3D11DeviceContext* context = CE_Engine::GetGPUContext().GetContext();
	unsigned int stride = myVertexBuffer->myVertexSize;
	unsigned int offset = 0;

	context->IASetVertexBuffers(0, 1, &myVertexBuffer->myBuffer, &stride, &offset);
	context->IASetIndexBuffer(myIndexBuffer->myBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->DrawIndexed(myIndexBuffer->myIndexCount, 0, 0);
}

void CE_RenderObject::CreateObjectData(int aBufferIndex, int aDataSize)
{
	myObjectBuffer = new CE_ConstantBuffer();
	myObjectBuffer->Init(aBufferIndex, aDataSize);
}

void CE_RenderObject::SetObjectData(void* someData, int aDataSize)
{
	CE_ASSERT(myObjectBuffer != nullptr, "Dont have ObjectBuffer");
	myObjectBuffer->SetData(someData, aDataSize);
	myObjectBuffer->SendToGPU();
}

void CE_RenderObject::UpdateVertexBuffer(void* someData, int aElementCount, int aElementSize)
{
	CE_ASSERT(myVertexBuffer != nullptr, "Dont have VertexBuffer");
	myVertexBuffer->UpdateDynamic(someData, aElementCount, aElementSize);
}
