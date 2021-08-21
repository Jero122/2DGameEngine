#include "Geometry2D.h"
#include <cmath>
#include <cfloat>

#include "glm/geometric.hpp"
#include "glm/gtx/norm.inl"

#define CMP(x, y) \
   (fabsf((x)–(y)) <= FLT_EPSILON * \
   fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))

float Length(const Line2D& line)
{
	return glm::length(line.start - line.end);
}

float LengthSquared(const Line2D& line)
{
	return glm::distance2(line.start, line.end);
}


glm::vec2 GetMin(const Rectangle2D& rect)
{
	glm::vec2 p1 = rect.position;
	glm::vec2 p2 = rect.position + rect.size;

	return glm::vec2(fminf(p1.x, p2.x), fminf(p1.y, p2.y));
}

glm::vec2 GetMax(const Rectangle2D& rect)
{
	glm::vec2 p1 = rect.position;
	glm::vec2 p2 = rect.position + rect.size;

	return glm::vec2(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y));
}

Rectangle2D FromMinMax(const glm::vec2& min, const glm::vec2& max)
{
	return Rectangle2D(min, max - min);
}
