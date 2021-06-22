#pragma once


#include <random>
#include <stack>
#include <vector>
#include "Vector2.h"
#include "Sprite.h"


class Map
{
private:
	double seed;
	int breadth;
	int distance;
	int length;
	double mapLength;
	std::vector<Vector2> map;
public:

	Map(const double seed = 1.1, const int breadth = 2, const int distance = 5, const int length = 7);
	~Map();

	void SetParameter(const double seed, const int breadth, const int distance, const int length);
	void MakeMap();

	inline std::vector<Vector2> GetMaps() const
	{
		return map;
	}

	inline bool GetMap(const Vector2 pos) const
	{
		if (std::find(map.begin(), map.end(), pos) != map.end())
		{
			return true;
		}

		return false;
	}

	inline const double GetLength()
	{
		return this->mapLength;
	}
};