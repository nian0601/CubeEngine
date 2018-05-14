#include "stdafx.h"
#include "CE_Text.h"

#include <d3d11.h>
#include "CE_GPUContext.h"
#include "CE_Font.h"
#include "CE_Texture.h"
#include "CE_MSDFFont.h"

CE_Text::CE_Text(CE_GPUContext& aGPUContext)
	: myGPUContext(aGPUContext)
	, myFont(nullptr)
	, myMSDFFont(nullptr)
	, myGotText(false)
{
}

CE_Text::~CE_Text()
{
	CE_SAFE_DELETE(myFont);
}

void CE_Text::Init()
{
	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", myGPUContext);

	SetupObjectBuffer();
}

void CE_Text::InitMSDF()
{
	myMSDFFont = new CE_MSDFFont();
	myMSDFFont->LoadFromFile("Data/Font/Consolas.ttf", myGPUContext);

	SetupObjectBuffer();
}

void CE_Text::Render()
{
	if (!myGotText)
		return;

	float scale = 1.f;
	if (myFont)
		scale = 0.33f;

	float maxHeight = 0.f;
	if (myFont)
		maxHeight = myFont->GetMaxHeight();
	else if (myMSDFFont)
		maxHeight = myMSDFFont->GetMaxHeight();

	ID3D11DeviceContext* context = myGPUContext.GetContext();

	//Update constant-buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	HRESULT result = context->Map(myObjectDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CE_ASSERT(SUCCEEDED(result), "Mapping failed");
	CE_Vector2f scaledSize;
	scaledSize.x = scale;
	scaledSize.y = scale;

	CE_Vector2f scaledPosition;
	scaledPosition.x = CE_RoundToInt(myPosition.x);
	scaledPosition.y = CE_RoundToInt(myPosition.y + maxHeight);

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

	context->PSSetShaderResources(0, 1, &shaderResource);
	// Set Vertex/Indexbuffers and Topology
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	context->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// Actually render
	context->DrawIndexed(myIndexCount, 0, 0);
}

void CE_Text::SetPosition(const CE_Vector2f& aPosition)
{
	myPosition = aPosition;
	if (myMSDFFont)
		myPosition.y -= myMSDFFont->GetMaxHeight();
}

void CE_Text::SetText(const CE_String& aString)
{
	myGotText = (aString.Lenght() + 1) > 0;

	CE_GrowingArray<VertexType> vertices;
	CE_GrowingArray<int> indices;

	if (myFont)
		BuildNormalText(aString, vertices, indices);
	else
		BuildMSDFText(aString, vertices, indices);

	myVertexCount = vertices.Size();
	myIndexCount = indices.Size();

	if(myVertexCount > 0)
		SetupVertexAndIndexBuffers(vertices.GetArrayAsPointer(), indices.GetArrayAsPointer());
}

void CE_Text::SetupObjectBuffer()
{
	D3D11_BUFFER_DESC objectDataBufferDesc;
	objectDataBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	objectDataBufferDesc.ByteWidth = sizeof(ObjectData);
	objectDataBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	objectDataBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	objectDataBufferDesc.MiscFlags = 0;
	objectDataBufferDesc.StructureByteStride = 0;

	myGPUContext.GetDevice()->CreateBuffer(&objectDataBufferDesc, NULL, &myObjectDataBuffer);
}

void CE_Text::SetupVertexAndIndexBuffers(void* aVertexData, void* aIndexData)
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

	CE_SAFE_RELEASE(myVertexBuffer);
	HRESULT result = myGPUContext.GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer);
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
	result = myGPUContext.GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &myIndexBuffer);
	if (FAILED(result))
		return;
}

void CE_Text::BuildNormalText(const CE_String& aString, CE_GrowingArray<VertexType>& someVertices, CE_GrowingArray<int>& someIndices)
{
	int numOfLetters = aString.Lenght() + 1;
	float drawX = 0;
	float drawY = 0;
	float z = 1.f;
	float height = 0.f;

	VertexType vert;
	for (int i = 0; i < numOfLetters; ++i)
	{
		CE_CharData charData;
		if (!myFont->GetCharData(aString[i], &charData))
			continue;

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

		vert.myPosition = CE_Vector4f(left, top, z, 1.f);
		vert.myUV = charData.myTopLeftUV;
		someVertices.Add(vert);

		vert.myPosition = CE_Vector4f(right, bottom, z, 1.f);
		vert.myUV = charData.myBottomRightUV;
		someVertices.Add(vert);

		vert.myPosition = CE_Vector4f(left, bottom, z, 1.f);
		vert.myUV = { charData.myTopLeftUV.x, charData.myBottomRightUV.y };
		someVertices.Add(vert);

		vert.myPosition = CE_Vector4f(right, top, z, 1.f);
		vert.myUV = { charData.myBottomRightUV.x, charData.myTopLeftUV.y };
		someVertices.Add(vert);


		int startIndex = i * 4;
		someIndices.Add(startIndex + 0);
		someIndices.Add(startIndex + 1);
		someIndices.Add(startIndex + 2);

		someIndices.Add(startIndex + 0);
		someIndices.Add(startIndex + 3);
		someIndices.Add(startIndex + 1);

		drawX += charData.myXAdvance;
		z -= 0.001f;
	}
}

void CE_Text::BuildMSDFText(const CE_String& aString, CE_GrowingArray<VertexType>& someVertices, CE_GrowingArray<int>& someIndices)
{
	int numOfLetters = aString.Lenght() + 1;
	float drawX = 0;
	float drawY = 0;
	float z = 1.f;
	float height = 0.f;

	VertexType vert;
	for (int i = 0; i < numOfLetters; ++i)
	{
		CE_MSDFGlyphData charData;
		if (!myMSDFFont->GetGlyphData(aString[i], &charData))
			return;

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

		vert.myPosition = CE_Vector4f(left, top, z, 1.f);
		vert.myUV = charData.myTopLeftUV;
		someVertices.Add(vert);

		vert.myPosition = CE_Vector4f(right, bottom, z, 1.f);
		vert.myUV = charData.myBottomRightUV;
		someVertices.Add(vert);

		vert.myPosition = CE_Vector4f(left, bottom, z, 1.f);
		vert.myUV = { charData.myTopLeftUV.x, charData.myBottomRightUV.y };
		someVertices.Add(vert);

		vert.myPosition = CE_Vector4f(right, top, z, 1.f);
		vert.myUV = { charData.myBottomRightUV.x, charData.myTopLeftUV.y };
		someVertices.Add(vert);


		int startIndex = i * 4;
		someIndices.Add(startIndex + 0);
		someIndices.Add(startIndex + 1);
		someIndices.Add(startIndex + 2);

		someIndices.Add(startIndex + 0);
		someIndices.Add(startIndex + 3);
		someIndices.Add(startIndex + 1);


		drawX += charData.myXAdvance;
		z -= 0.001f;
	}
}
