#pragma once

#include "CE_WindowMessage.h"

class CE_GPUContext;
class CE_Window;
class CE_WindowManager
{
public:
	static void Create(CE_GPUContext& aGPUContext);
	static void Destory();
	static CE_WindowManager* GetInstance();

	CE_Window* CreateNewWindow(const CE_Vector2i& aSize, const char* aTitle);

	bool PumpEvent();
	void HandleWindowMessage(CE_Window* aWindow, const CE_WindowMessage& aMessage);

	const CE_GrowingArray<CE_Window*>& GetWindows() const { return myWindows; }
	CE_GPUContext& GetGPUContext() { return myGPUContext; }

private:
	CE_WindowManager(CE_GPUContext& aGPUContext);
	~CE_WindowManager();

	CE_GPUContext& myGPUContext;
	CE_GrowingArray<CE_Window*> myWindows;

	static CE_WindowManager* ourInstance;
};

