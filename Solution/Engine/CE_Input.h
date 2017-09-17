#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "CE_Vector2.h"

class CE_Input
{
public:
	CE_Input(HWND aHwnd, HINSTANCE aHInstance, DWORD aKeyCoopFlags, DWORD aMouseCoopFlags);
	~CE_Input();

	float GetMouseDX() const;
	float GetMouseDY() const;
	float GetMouseDZ() const;

	const CE_Vector2f& GetMousePosition() const;

	bool MouseDown(int aButton) const;
	bool MouseUp(int aButton) const;
	bool MouseIsPressed(int aButton) const;

	bool KeyDown(unsigned int aKey) const;
	bool KeyUp(unsigned int aKey) const;
	bool KeyIsPressed(unsigned int aKey) const;

	void Update();
	void ToggleWindowActive();

	void TweakValue(float& aValue, float aRate, float aDeltaTime
		, unsigned int aIncreaseKey, unsigned int aDecreaseKey) const;

private:
	void CapturePreviousState();

	LPDIRECTINPUT8 myDirectInput;
	LPDIRECTINPUTDEVICE8 myKeyboardDevice;
	UCHAR myKeyState[256];
	UCHAR myPreviousKeyState[256];

	LPDIRECTINPUTDEVICE8 myMouseDevice;
	DIMOUSESTATE myMouseState;
	DIMOUSESTATE myPreviousMouseState;

	CE_Vector2f myMousePos;

	HWND myWindowHandler;

	bool myIsRecordingDeltas;
	bool myWindowIsActive;
};