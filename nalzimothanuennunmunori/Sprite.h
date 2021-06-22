#pragma once


#define _USE_MATH_DEFINES

#include <vector>
#include <fstream>
#include <Windows.h>
#include <algorithm>
#include <math.h>
#include "Vector2.h"
#include "Color.h"


class Sprite
{
private:
	const std::string					path;
	Vector2								size;
	std::vector<COLORREF>			colorSet;

public:
	

													Sprite(const std::string path);
													~Sprite();


	void											InitSprite();

	inline const Vector2							GetSize()
	{
		return size;
	}

	inline const COLORREF							GetColor(const Vector2 position)
	{
		return colorSet[(int)position.y * (int)this->size.x + (int)position.x];
	}

	inline const std::vector<std::pair<Vector2, COLORREF>>	GetSprite(const Vector2 position, float euler = 0) const
	{
		std::vector<std::pair<Vector2, COLORREF>> result;
		for (int y = 0; y < this->size.y; y++)
		{
			for (int x = 0; x < this->size.x; x++)
			{
				if (this->colorSet[y * (int)this->size.x + x])
				{
					const double rotation = M_PI * euler / 180;
					const double X = x - this->size.x / 2;
					const double Y = y - this->size.y / 2;

					result.emplace_back(std::make_pair(Vector2(floor(cos(rotation) * X + sin(rotation) * Y), floor(cos(rotation) * Y - sin(rotation) * X)) + position, this->colorSet[y * (int)this->size.x + x]));
				}
			}
		}
		return result;
	}

};