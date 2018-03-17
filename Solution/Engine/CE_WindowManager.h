#pragma once

struct CE_WindowMessage
{
	enum eType
	{
		NONE,
		PAINT,
		DESTROY,
		ACTIVATE,
		SIZE,
		ENTER_SIZE_MOVE,
		EXIT_SIZE_MOVE,
	};

	eType myType;

	int myLowWordLParam;
	int myHighWordLParam;
	int myLowWordWParam;
	int myHighWordWParam;
};

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

