#pragma once


#include "Screen.h"
#include "Animator.h"
#include "Object.h"


class Enemy
{
public:
	Object object;
	Animator animator;
	Vector2 tilePosition;
	Vector2 destination;

	bool collisionX;
	bool collisionY;

	int hp;
	int speed;
	int damage;

	bool rot; // true Left false Right

	Enemy();
	~Enemy();

	inline void SetEnemyDestination(Vector2 destination)
	{
		this->destination = destination;
	};

	inline void DrawEnemy(Screen& screen)
	{
		if (this->destination.x > this->tilePosition.x)
		{
			this->rot = false;
		}
		else
		{
			this->rot = true;
		}

		if (this->rot)
		{
			this->animator.SetAnimation("Left_Walk");
		}
		else
		{
			this->animator.SetAnimation("Right_Walk");
		}
		this->object.SetSprite(this->animator.GetAnimation());

		this->object.Draw(screen);
	}

};

