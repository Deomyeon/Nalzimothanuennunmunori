#pragma once


#include <vector>
#include <set>
#include "Sprite.h"
#include "Screen.h"
#include "Vector2.h"


class Object
{
private:

	Sprite* sprite;
	bool staticObject;

public:

	Vector2 position;
	float rotation;
	Vector2 size;

public:
	
	Object();
	Object(std::string sprite, Vector2 position = Vector2::zero, float rotation = 0, Vector2 size = Vector2::zero);
	~Object();

	inline bool Collision(const Vector2 position, const Vector2 size) const
	{
		if (this->size == Vector2::zero)
			return false;
		const Vector2 min1 = this->position - this->size / 2;
		const Vector2 max1 = this->position + this->size / 2 + Vector2::one;
		const Vector2 min2 = position - size / 2;
		const Vector2 max2 = position + size / 2 + Vector2::one;
		if (min1.x < max2.x && min1.y < max2.y && max1.x > min2.x && max1.y > min2.y)
		{
			return true;
		}
		return false;
	}

	inline void SetSprite(Sprite* sprite)
	{
		if (staticObject)
		{
			delete this->sprite;
		}
		this->sprite = sprite;
	}

	inline void Draw(Screen& screen) const
	{
		std::vector<std::pair<Vector2, COLORREF>> img = this->sprite->GetSprite(this->position, this->rotation);
		for (int i = 0; i < img.size(); i++)
		{
			screen.SetPixelInScreen(img[i].first, img[i].second);
		}
	}
};