#include "stdafx.h"

#include "CE_GPUContext.h"
#include "CE_FullscreenQuad.h"

#include <d3d11.h>
#include "CE_GBuffer.h"
#include "CE_Texture.h"

CE_FullscreenQuad::CE_FullscreenQuad()
	: myVertexBuffer(nullptr)
	, myIndexBuffer(nullptr)
{
}


CE_FullscreenQuad::~CE_FullscreenQuad()
{
	CE_SAFE_RELEASE(myIndexBuffer);
	CE_SAFE_RELEASE(myVertexBuffer);
}

void CE_FullscreenQuad::Init(const CE_GPUContext& aGPUContext)
{
	myVertexCount = 4;
	VertexType* vertices = new VertexType[myVertexCount];
	vertices[0].myPosition = CE_Vector3f(-1.0f, -1.0f, 0.0f); //topleft
	vertices[0].myTexCoord = CE_Vector2f(0.f, 1.f);

	vertices[1].myPosition = CE_Vector3f(1.0f, -1.0f, 0.0f); //topright
	vertices[1].myTexCoord = CE_Vector2f(1.f, 1.f);

	vertices[2].myPosition = CE_Vector3f(-1.0f, 1.0f, 0.0f); //bottomleft
	vertices[2].myTexCoord = CE_Vector2f(0.f, 0.f);

	vertices[3].myPosition = CE_Vector3f(1.0f, 1.0f, 0.0f); //bottomright
	vertices[3].myTexCoord = CE_Vector2f(1.f, 0.f);

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

void CE_FullscreenQuad::Render(const CE_GPUContext& aGPUContext, const CE_GBuffer& aGBuffer)
{
	ID3D11DeviceContext* context = aGPUContext.GetContext();

	ID3D11ShaderResourceView* resources[3];
	resources[0] = aGBuffer.myTextures[CE_GBuffer::ALBEDO_METALNESS]->GetShaderView();
	resources[1] = aGBuffer.myTextures[CE_GBuffer::NORMAL_ROUGNESS]->GetShaderView();
	resources[2] = aGBuffer.myTextures[CE_GBuffer::DEPTH]->GetShaderView();

	context->PSSetShaderResources(0, 3, resources);

	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	context->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Actually render
	context->DrawIndexed(myIndexCount, 0, 0);
}

void CE_FullscreenQuad::InitBuffers(const CE_GPUContext& aGPUContext, void* aVertexData, void* aIndexData)
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
	CE_ASSERT(SUCCEEDED(result), "Failed to create VertexBuffer");

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
	CE_ASSERT(SUCCEEDED(result), "Failed to create IndexBuffer");
}
