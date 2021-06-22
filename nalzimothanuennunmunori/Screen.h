#pragma once

#include <Windows.h>
#include <array>
#include <set>
#include "Vector2.h"
#include "Color.h"


class Screen
{
private:
	std::set<int>			fixedPixel;
	std::array<HANDLE, 2>	handle; // 콘솔 핸들
	std::array<Color, 2>	colorToColor; // 두 색
	Vector2					size;	// 화면 크기
	WORD*					screen; // 화면
	const char*				title; // 콘솔 타이틀
	unsigned char			screenHandle;

public:

							Screen(const Vector2 size, const char* const title, std::array<Color, 2> colorToColor);
							~Screen();

	void					SetConsole() const; // 콘솔 설정
	void					ResetConsole() const; // 콘솔 초기화

	inline const Vector2	GetScreenSize() const // 화면 크기 리턴
	{
		return this->size;
	}
	

	void					ReSize(const Vector2 size);
	void					SetColorToColor(const std::array<Color, 2> colorToColor);

	inline void				SetPixelInScreen(const Vector2 position, const WORD color) // (x, y) 위치의 픽셀을 color 값으로 바꿈.
	{
		if ((int)position.x >= 0 && (int)position.y >= 0 && (int)position.x <= this->size.x - 1 && (int)position.y <= this->size.y - 1)
		{
			int dot = ((int)this->size.y - 1 - (int)position.y) * (int)this->size.x + (int)position.x;
			this->screen[dot] = color;
			this->fixedPixel.insert(dot);
		}
	}
	inline const WORD		GetPixelInScreen(const Vector2 position) // (x, y) 위치의 픽셀을 얻어옴.
	{
		return this->screen[((int)this->size.y - 1 - (int)position.y) * (int)this->size.x + (int)position.x];
	}

	inline void				DrawScreen() // 화면 출력
	{
		DWORD bytesWritten = 0;

		screenHandle = !screenHandle;
		WriteConsoleOutputAttribute(this->handle[screenHandle], this->screen, (int)this->size.x * (int)this->size.y, { 0, 0 }, &bytesWritten);
		SetConsoleActiveScreenBuffer(this->handle[screenHandle]);
		for (std::set<int>::const_iterator it = this->fixedPixel.cbegin(); it != this->fixedPixel.cend(); )
		{
			this->screen[*it] = 0;
			std::set<int>::const_iterator it1 = it;
			it++;
			this->fixedPixel.erase(it1);
		}
	}
};