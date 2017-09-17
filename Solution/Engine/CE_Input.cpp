#include "stdafx.h"

#include "CE_Input.h"

CE_Input::CE_Input(HWND aHwnd, HINSTANCE aHInstance, DWORD aKeyCoopFlags, DWORD aMouseCoopFlags)
	: myWindowIsActive(true)
{
	DirectInput8Create(aHInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&myDirectInput), 0);

	myDirectInput->CreateDevice(GUID_SysKeyboard, &myKeyboardDevice, nullptr);
	myDirectInput->CreateDevice(GUID_SysMouse, &myMouseDevice, nullptr);

	myKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	myMouseDevice->SetDataFormat(&c_dfDIMouse);

	myKeyboardDevice->SetCooperativeLevel(aHwnd, aKeyCoopFlags);
	myMouseDevice->SetCooperativeLevel(aHwnd, aMouseCoopFlags);

	myKeyboardDevice->Acquire();
	myMouseDevice->Acquire();

	myWindowHandler = aHwnd;

	myIsRecordingDeltas = true;

	Update();
}

CE_Input::~CE_Input()
{
	myKeyboardDevice->Unacquire();
	myMouseDevice->Unacquire();
}

void CE_Input::Update()
{
	if (myWindowIsActive == true)
	{
		CapturePreviousState();
		HRESULT hr = myKeyboardDevice->GetDeviceState(sizeof(myKeyState), reinterpret_cast<void**>(&myKeyState));

		if (FAILED(hr))
		{
			ZeroMemory(myKeyState, sizeof(myKeyState));

			myKeyboardDevice->Acquire();
		}

		hr = myMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), reinterpret_cast<void**>(&myMouseState));
		if (FAILED(hr))
		{
			ZeroMemory(&myMouseState, sizeof(myMouseState));

			myMouseDevice->Acquire();
		}

		tagPOINT cursorPoint;
		GetCursorPos(&cursorPoint);
		ScreenToClient(myWindowHandler, &cursorPoint);
		myMousePos.x = static_cast<float>(cursorPoint.x);
		myMousePos.y = static_cast<float>(cursorPoint.y);

		if (myIsRecordingDeltas == false)
		{
			myMouseState.lX = 0;
			myMouseState.lY = 0;
		}
	}
}

void CE_Input::ToggleWindowActive()
{
	myWindowIsActive = !myWindowIsActive;
}

void CE_Input::TweakValue(float& aValue, float aRate, float aDeltaTime
	, unsigned int aIncreaseKey, unsigned int aDecreaseKey) const
{
	if (KeyIsPressed(aIncreaseKey))
	{
		aValue += aRate * aDeltaTime;
	}
	else if (KeyIsPressed(aDecreaseKey))
	{
		aValue -= aRate * aDeltaTime;
	}
}

const CE_Vector2f& CE_Input::GetMousePosition() const
{
	return myMousePos;
}

void CE_Input::CapturePreviousState()
{
	memcpy_s(myPreviousKeyState, sizeof(myPreviousKeyState), myKeyState, sizeof(myKeyState));
	memcpy_s(&myPreviousMouseState, sizeof(myPreviousMouseState), &myMouseState, sizeof(myMouseState));
}

bool CE_Input::KeyIsPressed(unsigned int aKey) const
{
	UCHAR key = static_cast<UCHAR>(aKey);
	return (myKeyState[key] & 0x80) != 0;
}

bool CE_Input::KeyDown(unsigned int aKey) const
{
	UCHAR key = static_cast<UCHAR>(aKey);
	return ((myKeyState[key] & 0x80) != 0 && (myPreviousKeyState[key] & 0x80) == 0);
}

bool CE_Input::KeyUp(unsigned int aKey) const
{
	UCHAR key = static_cast<UCHAR>(aKey);
	return ((myKeyState[key] & 0x80) == 0 && (myPreviousKeyState[key] & 0x80) != 0);
}

bool CE_Input::MouseIsPressed(int aButton) const
{
	return (myMouseState.rgbButtons[aButton] & 0x80) != 0;
}

bool CE_Input::MouseDown(int aButton) const
{
	return ((myMouseState.rgbButtons[aButton] & 0x80) != 0 && (myPreviousMouseState.rgbButtons[aButton] & 0x80) == 0);
}

bool CE_Input::MouseUp(int aButton) const
{
	return ((myMouseState.rgbButtons[aButton] & 0x80) == 0 && (myPreviousMouseState.rgbButtons[aButton] & 0x80) != 0);
}

float CE_Input::GetMouseDX() const
{
	return static_cast<float>(myMouseState.lX);
}

float CE_Input::GetMouseDY() const
{
	return static_cast<float>(myMouseState.lY);
}

float CE_Input::GetMouseDZ() const
{
	return static_cast<float>(myMouseState.lZ);
}