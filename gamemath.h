#pragma once

const float PI = 3.141592654;

struct Vector2f
{
	float x;
	float y;
};


Vector2f operator*(const Vector2f& v, const float n);
Vector2f operator+=(Vector2f& a, const Vector2f& b);
Vector2f operator/=(Vector2f& a, const float b);
Vector2f operator-(const Vector2f& a, const Vector2f& b);

void rotateVectorAroundVector(Vector2f* a, Vector2f* origin, double angle);
double degToRad(double deg);

int random(int min, int max);
bool isPointInCircle(Vector2f point, Vector2f circleCenter, int circleRadius);
