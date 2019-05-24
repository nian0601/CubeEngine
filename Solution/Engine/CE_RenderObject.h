#pragma once
#include "CE_GPUBuffer.h"


class CE_GPUBuffer;
class CE_GPUContext;

class CE_RenderObject
{
public:
	CE_RenderObject();
	~CE_RenderObject();

	void InitCube(const CE_GPUContext& aGPUContext);
	void InitSprite(const CE_GPUContext& aGPUContext);
	void InitFullscreenQuad(const CE_GPUContext& aGPUContext);
	void InitSphere(const CE_GPUContext& aGPUContext);
	void InitLightSphere(const CE_GPUContext& aGPUContext);

	template<typename VertexType>
	void Init(const CE_GPUContext& aGPUContext, void* someVertexData, int aVertexCount, void* someIndexData, int aIndexCount);

	void Render();

	void CreateObjectData(unsigned int aSize, int aBufferIndex);
	template <typename T>
	T* GetObjectData();

private:
	CE_GPUBuffer* myGPUBuffer;
};

template<typename VertexType>
void CE_RenderObject::Init(const CE_GPUContext& aGPUContext, void* someVertexData, int aVertexCount, void* someIndexData, int aIndexCount)
{
	myGPUBuffer = new CE_GPUBuffer(aGPUContext);
	myGPUBuffer->InitVertexBuffer(someVertexData, aVertexCount, sizeof(VertexType));
	myGPUBuffer->InitIndexBuffer(someIndexData, aIndexCount);
	myGPUBuffer->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

inline void CE_RenderObject::CreateObjectData(unsigned int aSize, int aBufferIndex)
{
	CE_ASSERT(myGPUBuffer != nullptr, "Dont have GPUBuffer");
	myGPUBuffer->InitObjectData(aSize, aBufferIndex);
}

template <typename T>
T* CE_RenderObject::GetObjectData()
{
	CE_ASSERT(myGPUBuffer != nullptr, "Dont have GPUBuffer");
	return static_cast<T*>(myGPUBuffer->GetObjectData());
}
