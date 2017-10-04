#pragma once


class CE_Window;
class CE_WindowManager
{
public:
	CE_WindowManager();
	~CE_WindowManager();

	CE_Window* CreateNewWindow();

private:
	CE_GrowingArray<CE_Window*> myWindows;
};

