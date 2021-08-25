#pragma once
#include "glm/vec2.hpp"

class Shape
{


public:
	float density = 1.0f;
	float friction = 0.3f;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	
	Shape(float density, float friction)
		: density(density),
		  friction(friction)
	{
	}
};

class OrientedBox : public Shape
{
public:
	float halfExtentX;
	float halfExtentY;

	float angle = 0.0f; //Radians

	OrientedBox(float half_extent_x, float half_extent_y, float density, float friction, float degrees)
		: Shape(density, friction),
		  halfExtentX(half_extent_x),
		  halfExtentY(half_extent_y),
		  angle(degrees)
	{
	}

	OrientedBox( float half_extent_x, float density, float friction, float half_extent_y)
		: Shape(density, friction),
		  halfExtentX(half_extent_x),
		  halfExtentY(half_extent_y)
	{
	}
};

class Circle : public Shape
{
public:
	float radius;

	Circle(float density, float friction, float radius)
		: Shape(density, friction),
		  radius(radius)
	{
	}
};
