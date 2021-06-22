#pragma once

#include <vector>
#include <array>
#include <algorithm>

class Vector2
{
public:
	float x;
	float y;
	static const Vector2 zero;
	static const Vector2 one;

	Vector2();
	Vector2(const float x, const float y);
	~Vector2();

	const std::vector<Vector2> GetLine(const Vector2 target);
	inline const float GetDistance(const Vector2 target)
	{
		return sqrtf(powf(target.x - this->x, 2) + powf(target.y - this->y, 2));
	}

	static const std::vector<Vector2> GetMeshTriangle(std::array<Vector2, 3>& triangle);
	static const std::vector<Vector2> GetWireFrameTriangle(std::array<Vector2, 3>& triangle);


	inline const Vector2 operator +(const Vector2 target) const
	{
		return Vector2(this->x + target.x, this->y + target.y);
	}
	inline const Vector2 operator -(const Vector2 target) const
	{
		return Vector2(this->x - target.x, this->y - target.y);
	}
	inline const Vector2 operator *(const float target) const
	{
		return Vector2(this->x * target, this->y * target);
	}
	inline const Vector2 operator /(const float target) const
	{
		return Vector2(this->x / target, this->y / target);
	}
	inline const Vector2 operator =(const Vector2 target)
	{
		this->x = target.x;
		this->y = target.y;
		return *this;
	}
	inline const bool operator ==(const Vector2 target) const
	{
		return (this->x == target.x && this->y == target.y);
	}
	inline const bool operator <(const Vector2 target) const
	{
		return (this->x < target.x && this->y < target.y);
	}
	inline const bool operator >(const Vector2 target) const
	{
		return (this->x > target.x&& this->y > target.y);
	}
	inline const bool operator <=(const Vector2 target) const
	{
		return (this->x <= target.x&& this->y <= target.y);
	}
	inline const bool operator >=(const Vector2 target) const
	{
		return (this->x >= target.x && this->y >= target.y);
	}
	inline const bool operator !=(const Vector2 target) const
	{
		return (this->x != target.x && this->y != target.y);
	}

};