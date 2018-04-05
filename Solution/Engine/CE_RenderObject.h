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

	void Render();

	void CreateObjectData(unsigned int aSize, int aBufferIndex);
	template <typename T>
	T* GetObjectData();

private:
	CE_GPUBuffer* myGPUBuffer;
};

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
