#pragma once
#include "glm/vec2.hpp"

typedef glm::vec2 Point2D;

struct Line2D
{
	Point2D start;
	Point2D end;

	inline Line2D(){}
	inline Line2D(const Point2D& start, const Point2D& end)
		: start(start), end(end)
	{}
};

struct Circle
{
	Point2D position;
	float radius;

	inline Circle() : radius(1.0f){}
	inline Circle(const Point2D& position, float radius) :
		position(position), radius(radius) {}
};


struct Rectangle2D
{
	Point2D position;
	glm::vec2 size;

	inline Rectangle2D() : size(1,1) { }
	inline Rectangle2D(const Point2D& position, const glm::vec2& size):
		position(position), size(size) { }
};

struct OrientedRectangle2D
{
	Point2D position;
	glm::vec2 halfExtents;
	float rotation;

	inline OrientedRectangle2D() :
		halfExtents(1.0f, 1.0f), rotation(0.0f) { }
	inline OrientedRectangle2D(const Point2D& position, const glm::vec2& halfExtents) :
		position(position), halfExtents(halfExtents), rotation(0.0f) { }
};


float Length(const Line2D& line);
float LengthSquared(const Line2D& line);

glm::vec2 GetMin(const Rectangle2D& rect);
glm::vec2 GetMax(const Rectangle2D& rect);
Rectangle2D FromMinMax(const glm::vec2& min, const glm::vec2& max);