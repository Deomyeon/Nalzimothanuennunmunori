#include "Object.h"


Object::Object() : sprite(NULL), position(Vector2::zero), rotation(0), size(Vector2::zero), staticObject(false)
{

}

Object::Object(std::string sprite, Vector2 position, float rotation, Vector2 size) : sprite(new Sprite(sprite)), position(position), rotation(rotation), size(size), staticObject(true)
{
	this->sprite->InitSprite();
}

Object::~Object() 
{
	if (staticObject)
	{
		delete this->sprite;
	}
}