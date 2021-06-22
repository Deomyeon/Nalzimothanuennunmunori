#pragma once

#include <Windows.h>
#include <thread>
#include "Vector2.h"

DWORD WINAPI SetMouseInput(LPVOID param);

class MouseInput
{
public:
	Vector2 mousePos;
	bool left;
	bool right;

	bool stop;

	MouseInput();
	~MouseInput();

	inline void StartMouseInput()
	{
		HANDLE hThread = CreateThread(NULL, 0, SetMouseInput, this, NULL, 0);
	}

	inline void StopMouseInput()
	{
		this->stop = true;
	}

	inline const bool GetLeft() const
	{
		return this->left;
	}

	inline const bool GetRight() const
	{
		return this->right;
	}

	inline const Vector2 GetMousePos() const
	{
		return this->mousePos;
	}
};