
#pragma once

#include<cmath>

//2éüå≥ÉxÉNÉgÉã
template <class T>
class Vec2
{
public:
	T x;
	T y;

public:
	Vec2():
		x(static_cast<T>(0)),
		y(static_cast<T>(0))
	{

	}
	Vec2(T posX, T posY)
	{
		x = posX;
		y = posY;
	}

	//Vec2 = (Vec2 += Vec2)
	Vec2 operator+=(const Vec2& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}
	//Vec2 = Vec2 + Vec2
	Vec2 operator+(Vec2& vec)
	{
		Vec2 temp{ x + vec.x, y + vec.y };
		return temp;
	}

	//Vec2 = (Vec2 -= Vec2)
	Vec2 operator-=(const Vec2& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}
	//Vec2 = Vec2 - Vec2
	Vec2 operator-(Vec2& vec)
	{
		Vec2 temp{ x - vec.x, y - vec.y };
		return temp;
	}

	//Vec2 = (Vec2 *= Vec2)
	Vec2 operator*=(T scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}
	//Vec2 = Vec2 + float
	Vec2 operator*(T scale)const
	{
		Vec2 temp{ x * scale, y * scale };
		return temp;
	}

	//Vec2 = (Vec2 /= Vec2)
	Vec2 operator/=(T scale)
	{
		x /= scale;
		y /= scale;
		return *this;
	}
	//Vec2 = Vec2 / float
	Vec2 operator/(T scale)const
	{
		Vec2 temp{ x / scale, y / scale };
		return temp;
	}

	//í∑Ç≥ÇÃéÊìæ
	T Length()
	{
		return sqrtf((x * x) + (y * y));
	}

	//ê≥ãKâª Vec2 = Vec2.normalize()
	Vec2 Normalize()
	{
		const T len = Length();
		if (len == 0)
		{
			return *this;
		}
		return(*this) / len;
	}
};
