#include "gamemath.h"
#include <math.h>
#include <stdlib.h>

Vector2f operator*(const Vector2f& v, const float n)
{
	Vector2f vector;
	vector.x = v.x * n;
	vector.y = v.y * n;
	return vector;
}


Vector2f operator+=(Vector2f& a, const Vector2f& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

Vector2f operator/=(Vector2f& a, const float b)
{
	a.x /= b;
	a.y /= b;
	return a;
}

Vector2f operator-(const Vector2f& a, const Vector2f& b)
{
	Vector2f v;
	v.x = a.x - b.x;
	v.y = a.y - b.y;
	return v;
}

void rotateVectorAroundVector(Vector2f* a, Vector2f* origin, double angle)
{
	float x = (a->x - origin->x) * cosf(angle) - (a->y - origin->y) * sinf(angle) + origin->x;
	float y = (a->x - origin->x) * sinf(angle) + (a->y - origin->y) * cosf(angle) + origin->y;

	a->x = x;
	a->y = y;
}

double degToRad(double deg)
{
	return deg * (PI / 180);
}

int random(int min, int max)
{
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

bool isPointInCircle(Vector2f point, Vector2f circleCenter, int circleRadius)
{
	return powf((point.x - circleCenter.x), 2) + powf((point.y - circleCenter.y), 2) <= powf(circleRadius, 2);
}
