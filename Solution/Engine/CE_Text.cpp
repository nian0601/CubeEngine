#include "stdafx.h"
#include "CE_Text.h"

#include <d3d11.h>
#include "CE_GPUContext.h"
#include "CE_Font.h"
#include "CE_Texture.h"
#include "CE_MSDFFont.h"

CE_Text::CE_Text()
	: myFont(nullptr)
	, myMSDFFont(nullptr)
{
}

CE_Text::~CE_Text()
{
	CE_SAFE_DELETE(myFont);
}

void CE_Text::Init(CE_GPUContext& aGPUContext)
{
	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", aGPUContext);

	CE_String testString = "Herp Derp";
	int numOfLetters = testString.Lenght() + 1;
	float drawX = 0;
	float drawY = 0;
	float z = 1.f;
	float height = 0.f;

	CE_GrowingArray<VertexType> vertices;
	CE_GrowingArray<int> indices;

	VertexType vert;
	for (int i = 0; i < numOfLetters; ++i)
	{
		CE_CharData charData = myFont->GetCharData(testString[i]);
		if (charData.myHeight > height)
			height = static_cast<float>(charData.myHeight);


		float left = drawX + charData.myXOffset;
		float right = left + charData.myWidth;
		float top = drawY + charData.myYOffset;
		float bottom = top - charData.myHeight;

		left = static_cast<float>(static_cast<int>(left + 0.5f));
		right = static_cast<float>(static_cast<int>(right + 0.5f));
		top = static_cast<float>(static_cast<int>(top + 0.5f));
		bottom = static_cast<float>(static_cast<int>(bottom + 0.5f));

		vert.myPosition = CE_Vector3f(left, top, z);
		vert.myUV = charData.myTopLeftUV;
		vertices.Add(vert);

		vert.myPosition = CE_Vector3f(right, bottom, z);
		vert.myUV = charData.myBottomRightUV;
		vertices.Add(vert);

		vert.myPosition = CE_Vector3f(left, bottom, z);
		vert.myUV = { charData.myTopLeftUV.x, charData.myBottomRightUV.y };
		vertices.Add(vert);

		vert.myPosition = CE_Vector3f(right, top, z);
		vert.myUV = { charData.myBottomRightUV.x, charData.myTopLeftUV.y };
		vertices.Add(vert);


		int startIndex = i * 4;
		indices.Add(startIndex + 0);
		indices.Add(startIndex + 1);
		indices.Add(startIndex + 2);

		indices.Add(startIndex + 0);
		indices.Add(startIndex + 3);
		indices.Add(startIndex + 1);


		drawX += charData.myXAdvance;
		z -= 0.001f;
	}

	myVertexCount = vertices.Size();
	myIndexCount = indices.Size();

	SetupVertexAndIndexBuffers(aGPUContext, vertices.GetArrayAsPointer(), indices.GetArrayAsPointer());
	SetupObjectBuffer(aGPUContext);
}

void CE_Text::InitMSDF(CE_GPUContext& aGPUContext)
{
	myMSDFFont = new CE_MSDFFont();
	myMSDFFont->LoadFromFile("Data/Font/Consolas.ttf", aGPUContext);

	//CE_String testString = "ABCDEFGHI";
	//CE_String testString = "AAAAA";
	CE_String testString = "BBBBB";
	//CE_String testString = "123456789";
	int numOfLetters = testString.Lenght() + 1;
	float drawX = 0;
	float drawY = 0;
	float z = 1.f;
	float height = 0.f;

	CE_GrowingArray<VertexType> vertices;
	CE_GrowingArray<int> indices;

	VertexType vert;
	for (int i = 0; i < numOfLetters; ++i)
	{
		CE_MSDFGlyphData charData = myMSDFFont->GetGlyphData(testString[i]);
		if (charData.myHeight > height)
			height = static_cast<float>(charData.myHeight);


		float left = drawX;// +charData.myXOffset;
		float right = left + charData.myWidth;
		float top = drawY;// +charData.myYOffset;
		float bottom = top - charData.myHeight;

		left = static_cast<float>(static_cast<int>(left + 0.5f));
		right = static_cast<float>(static_cast<int>(right + 0.5f));
		top = static_cast<float>(static_cast<int>(top + 0.5f));
		bottom = static_cast<float>(static_cast<int>(bottom + 0.5f));

		vert.myPosition = CE_Vector3f(left, top, z);
		vert.myUV = charData.myTopLeftUV;
		vertices.Add(vert);

		vert.myPosition = CE_Vector3f(right, bottom, z);
		vert.myUV = charData.myBottomRightUV;
		vertices.Add(vert);

		vert.myPosition = CE_Vector3f(left, bottom, z);
		vert.myUV = { charData.myTopLeftUV.x, charData.myBottomRightUV.y };
		vertices.Add(vert);

		vert.myPosition = CE_Vector3f(right, top, z);
		vert.myUV = { charData.myBottomRightUV.x, charData.myTopLeftUV.y };
		vertices.Add(vert);


		int startIndex = i * 4;
		indices.Add(startIndex + 0);
		indices.Add(startIndex + 1);
		indices.Add(startIndex + 2);

		indices.Add(startIndex + 0);
		indices.Add(startIndex + 3);
		indices.Add(startIndex + 1);


		drawX += charData.myXAdvance;
		z -= 0.001f;
	}

	myVertexCount = vertices.Size();
	myIndexCount = indices.Size();

	SetupVertexAndIndexBuffers(aGPUContext, vertices.GetArrayAsPointer(), indices.GetArrayAsPointer());
	SetupObjectBuffer(aGPUContext);
}

void CE_Text::Render(const CE_GPUContext& aGPUContext)
{
	ID3D11DeviceContext* context = aGPUContext.GetContext();

	//Update constant-buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	context->Map(myObjectDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	const CE_Vector2i& windowSize = aGPUContext.GetWindowSize();

	CE_Vector2f scaledSize;
	scaledSize.x = 1.f; //windowSize.x * mySize.x;
	scaledSize.y = 1.f; //windowSize.y * mySize.y;
	//scaledSize *= 0.5f;

	CE_Vector2f scaledPosition;
	scaledPosition.x = windowSize.x * myPosition.x;
	scaledPosition.y = windowSize.y * myPosition.y;

	ObjectData* dataPtr = (ObjectData*)mappedResource.pData;
	dataPtr->myWorld = myOrientation;
	dataPtr->myColor = myColor;
	dataPtr->mySize = scaledSize;
	dataPtr->myPosition = scaledPosition;

	context->Unmap(myObjectDataBuffer, 0);

	context->VSSetConstantBuffers(1, 1, &myObjectDataBuffer);

	ID3D11ShaderResourceView* shaderResource = nullptr;
	if(myMSDFFont != nullptr)
		shaderResource = myMSDFFont->GetTexture()->GetShaderView();
	else
		shaderResource = myFont->GetTexture()->GetShaderView();

	aGPUContext.GetContext()->PSSetShaderResources(0, 1, &shaderResource);
	// Set Vertex/Indexbuffers and Topology
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	aGPUContext.GetContext()->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	aGPUContext.GetContext()->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	aGPUContext.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// Actually render
	context->DrawIndexed(GetIndexCount(), 0, 0);
}

void CE_Text::SetupObjectBuffer(const CE_GPUContext& aGPUContext)
{
	D3D11_BUFFER_DESC objectDataBufferDesc;
	objectDataBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	objectDataBufferDesc.ByteWidth = sizeof(ObjectData);
	objectDataBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	objectDataBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	objectDataBufferDesc.MiscFlags = 0;
	objectDataBufferDesc.StructureByteStride = 0;

	HRESULT result;
	result = aGPUContext.GetDevice()->CreateBuffer(&objectDataBufferDesc, NULL, &myObjectDataBuffer);
}

void CE_Text::SetupVertexAndIndexBuffers(const CE_GPUContext& aGPUContext, void* aVertexData, void* aIndexData)
{
	HRESULT result;

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

	CE_SAFE_RELEASE(myVertexBuffer);
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

	CE_SAFE_RELEASE(myIndexBuffer);
	result = aGPUContext.GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &myIndexBuffer);
	if (FAILED(result))
		return;
}
