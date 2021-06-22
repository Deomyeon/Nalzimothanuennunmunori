#include "MouseInput.h"


MouseInput::MouseInput() : mousePos(Vector2::zero), left(false), right(false), stop(false) {}

MouseInput::~MouseInput() {  }

DWORD WINAPI SetMouseInput(LPVOID param)
{
    while (true)
    {
        MouseInput* mouseInput = (MouseInput*)param;

		POINT mouse;
		GetCursorPos(&mouse);
		WINDOWINFO window;
		GetWindowInfo(GetConsoleWindow(), &window);
		int screenX = ((window.rcClient.right - window.rcClient.left) / 1.6 - 1);
		int screenY = ((window.rcClient.bottom - window.rcClient.top) / 1.6 - 1);
		int x = (mouse.x - window.rcClient.left) / 1.6;
		int y = ((mouse.y - window.rcClient.top - window.cyWindowBorders / 4) / 1.6);
		if (x < 0)
		{
			x = 0;
		}
		if (x > screenX)
		{
			x = screenX;
		}
		if (y < 0)
		{
			y = 0;
		}
		if (y > screenY)
		{
			y = screenY;
		}
		y = screenY - y;
		mouseInput->mousePos = Vector2(x, y);

		if (GetAsyncKeyState(MK_LBUTTON) & 0x8000)
		{
			mouseInput->left = true;
		}
		else
		{
			mouseInput->left = false;
		}

		if (GetAsyncKeyState(MK_RBUTTON) & 0x8000)
		{
			mouseInput->right = true;
		}
		else
		{
			mouseInput->right = false;
		}

		if (mouseInput->stop)
			return 0;
    }

    return 0;
}