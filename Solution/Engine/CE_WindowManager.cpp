#include "stdafx.h"

#include "CE_Window.h"
#include "CE_WindowManager.h"


CE_WindowManager::CE_WindowManager()
{
}


CE_WindowManager::~CE_WindowManager()
{
}

CE_Window* CE_WindowManager::CreateNewWindow()
{
	CE_Window* window = new CE_Window();
	myWindows.Add(window);

	return window;
}
