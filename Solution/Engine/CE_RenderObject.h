#pragma once

class CE_GPUIndexBuffer;
class CE_GPUVertexBuffer;
class CE_ConstantBuffer;

class CE_RenderObject
{
public:
	CE_RenderObject();
	~CE_RenderObject();

	void Init(void* someVertexData, int aVertexCount, int aVertexSize, void* someIndexData, int aIndexCount);
	void InitCube();
	void InitSprite();
	void InitFullscreenQuad();
	void InitSphere();
	void InitLightSphere();

	void Render();

	void CreateObjectData(int aBufferIndex, int aDataSize);
	void SetObjectData(void* someData, int aDataSize);

private:
	CE_GPUVertexBuffer* myVertexBuffer;
	CE_GPUIndexBuffer* myIndexBuffer;
	CE_ConstantBuffer* myObjectBuffer;
};