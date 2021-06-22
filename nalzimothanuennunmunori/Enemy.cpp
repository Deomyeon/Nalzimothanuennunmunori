#include "Enemy.h"


Enemy::Enemy() :
	object(),
	animator(),
	tilePosition(Vector2::zero),
	destination(Vector2::zero),
	hp(4),
	speed(10),
	damage(1),
	rot(true),
	collisionX(false),
	collisionY(false)
{
	this->animator.LoadAnimations(std::vector<std::string>{"Assets/baquee.animation"});
	this->object.position = Vector2::zero;
	this->object.rotation = 0;
	this->object.size = Vector2(32, 16);
}

Enemy::~Enemy()
{

}