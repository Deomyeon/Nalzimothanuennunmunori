#pragma once


#include <Windows.h>

class Color
{
public:
	BYTE r;
	BYTE g;
	BYTE b;

	static const Color black;
	static const Color white;
	static const Color red;
	static const Color green;
	static const Color blue;


	Color(const BYTE r, const BYTE g, const BYTE b);
	~Color();
};