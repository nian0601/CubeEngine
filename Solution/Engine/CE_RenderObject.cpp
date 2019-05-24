#include "stdafx.h"
#include "CE_RenderObject.h"
#include <d3d11.h>
#include "CE_GPUContext.h"
#include "CE_GPUBuffer.h"
#include "CE_ShaderStructs.h"
#include "CE_ModelSphereCreator.h"
#include "CE_LightSphereCreator.h"
#include "CE_ObjLoader.h"

CE_RenderObject::CE_RenderObject()
	: myGPUBuffer(nullptr)
{
}

CE_RenderObject::~CE_RenderObject()
{
	CE_SAFE_DELETE(myGPUBuffer);
}

void CE_RenderObject::InitCube(const CE_GPUContext& aGPUContext)
{
#pragma region Vertices
	int vertexCount = 24;
	CE_PosNormColor_Vert* vertices = new CE_PosNormColor_Vert[vertexCount];

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
	int indexCount = 36;
	UINT* indices = new UINT[indexCount];
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

	myGPUBuffer = new CE_GPUBuffer(aGPUContext);
	myGPUBuffer->InitVertexBuffer(vertices, vertexCount, sizeof(CE_PosNormColor_Vert));
	myGPUBuffer->InitIndexBuffer(indices, indexCount);
	myGPUBuffer->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CE_SAFE_DELETE_ARRAY(vertices);
	CE_SAFE_DELETE_ARRAY(indices);
}

void CE_RenderObject::InitSprite(const CE_GPUContext& aGPUContext)
{
	int vertexCount = 4;
	CE_Pos_Vert* vertices = new CE_Pos_Vert[vertexCount];
	vertices[0].myPosition = CE_Vector4f(-1.0f, -1.0f, 0.0f, 1.f); //topleft
	vertices[1].myPosition = CE_Vector4f(1.0f, -1.0f, 0.0f, 1.f); //topright
	vertices[2].myPosition = CE_Vector4f(-1.0f, 1.0f, 0.0f, 1.f); //bottomleft
	vertices[3].myPosition = CE_Vector4f(1.0f, 1.0f, 0.0f, 1.f); //bottomright

															// Load the index array with data.
	unsigned int indexCount = 6;
	UINT* indices = new UINT[indexCount];
	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;

	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	myGPUBuffer = new CE_GPUBuffer(aGPUContext);
	myGPUBuffer->InitVertexBuffer(vertices, vertexCount, sizeof(CE_Pos_Vert));
	myGPUBuffer->InitIndexBuffer(indices, indexCount);
	myGPUBuffer->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CE_SAFE_DELETE_ARRAY(vertices);
	CE_SAFE_DELETE_ARRAY(indices);
}

void CE_RenderObject::InitFullscreenQuad(const CE_GPUContext& aGPUContext)
{
	int vertexCount = 4;
	CE_Pos_UV_Vert* vertices = new CE_Pos_UV_Vert[vertexCount];
	vertices[0].myPosition = CE_Vector4f(-1.0f, -1.0f, 0.0f, 1.f); //topleft
	vertices[0].myTexCoord = CE_Vector2f(0.f, 1.f);

	vertices[1].myPosition = CE_Vector4f(1.0f, -1.0f, 0.0f, 1.f); //topright
	vertices[1].myTexCoord = CE_Vector2f(1.f, 1.f);

	vertices[2].myPosition = CE_Vector4f(-1.0f, 1.0f, 0.0f, 1.f); //bottomleft
	vertices[2].myTexCoord = CE_Vector2f(0.f, 0.f);

	vertices[3].myPosition = CE_Vector4f(1.0f, 1.0f, 0.0f, 1.f); //bottomright
	vertices[3].myTexCoord = CE_Vector2f(1.f, 0.f);

	int indexCount = 6;
	UINT* indices = new UINT[indexCount];
	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;

	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;;

	myGPUBuffer = new CE_GPUBuffer(aGPUContext);
	myGPUBuffer->InitVertexBuffer(vertices, vertexCount, sizeof(CE_Pos_UV_Vert));
	myGPUBuffer->InitIndexBuffer(indices, indexCount);
	myGPUBuffer->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CE_SAFE_DELETE_ARRAY(vertices);
	CE_SAFE_DELETE_ARRAY(indices);
}

void CE_RenderObject::InitSphere(const CE_GPUContext& aGPUContext)
{
	CE_ModelSphereCreator sphereCreator;
	sphereCreator.Create(4);

	myGPUBuffer = new CE_GPUBuffer(aGPUContext);
	myGPUBuffer->InitVertexBuffer(sphereCreator.myVertices.GetArrayAsPointer(), sphereCreator.myVertices.Size(), sizeof(sphereCreator.myVertices[0]));
	myGPUBuffer->InitIndexBuffer(sphereCreator.myIndices.GetArrayAsPointer(), sphereCreator.myIndices.Size());
	myGPUBuffer->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CE_RenderObject::InitLightSphere(const CE_GPUContext& aGPUContext)
{
	CE_LightSphereCreator sphereCreator;
	sphereCreator.Create(2);

	myGPUBuffer = new CE_GPUBuffer(aGPUContext);
	myGPUBuffer->InitVertexBuffer(sphereCreator.myVertices.GetArrayAsPointer(), sphereCreator.myVertices.Size(), sizeof(sphereCreator.myVertices[0]));
	myGPUBuffer->InitIndexBuffer(sphereCreator.myIndices.GetArrayAsPointer(), sphereCreator.myIndices.Size());
	myGPUBuffer->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CE_RenderObject::Render()
{
	myGPUBuffer->Render();
}