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
	std::array<HANDLE, 2>	handle; // �ܼ� �ڵ�
	std::array<Color, 2>	colorToColor; // �� ��
	Vector2					size;	// ȭ�� ũ��
	WORD*					screen; // ȭ��
	const char*				title; // �ܼ� Ÿ��Ʋ
	unsigned char			screenHandle;

public:

							Screen(const Vector2 size, const char* const title, std::array<Color, 2> colorToColor);
							~Screen();

	void					SetConsole() const; // �ܼ� ����
	void					ResetConsole() const; // �ܼ� �ʱ�ȭ

	inline const Vector2	GetScreenSize() const // ȭ�� ũ�� ����
	{
		return this->size;
	}
	

	void					ReSize(const Vector2 size);
	void					SetColorToColor(const std::array<Color, 2> colorToColor);

	inline void				SetPixelInScreen(const Vector2 position, const WORD color) // (x, y) ��ġ�� �ȼ��� color ������ �ٲ�.
	{
		if ((int)position.x >= 0 && (int)position.y >= 0 && (int)position.x <= this->size.x - 1 && (int)position.y <= this->size.y - 1)
		{
			int dot = ((int)this->size.y - 1 - (int)position.y) * (int)this->size.x + (int)position.x;
			this->screen[dot] = color;
			this->fixedPixel.insert(dot);
		}
	}
	inline const WORD		GetPixelInScreen(const Vector2 position) // (x, y) ��ġ�� �ȼ��� ����.
	{
		return this->screen[((int)this->size.y - 1 - (int)position.y) * (int)this->size.x + (int)position.x];
	}

	inline void				DrawScreen() // ȭ�� ���
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