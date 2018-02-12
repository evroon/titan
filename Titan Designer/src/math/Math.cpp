#include "Math.h"

#if 0

double Math::DegToRad(double degrees)
{
	return (degrees * PI) / 180;
}

double Math::RadToDeg(double radians)
{
	return (radians / PI) * 180;
}

double Math::GetAngle(const vec2 &pos)
{
	double res = (double)atan2(pos.y, pos.x);
	if (res >= 0.0)	return res;
	else			return TWOPI + res;
}

//get vector

//floor

//modulo

template<typename T>
int Math::sgn(T x)
{
	return x > 0.0 ? 1 : -1;
}

template<typename T>
double Math::distance(double a, double b)
{
	return abs(abs(a) - abs(b));
}

template<typename T>
double Math::Distance(const Vec2<T> &pos_1, const Vec2<T> &pos_2)
{
	return (double)sqrt((pos_1.x - pos_2.x) * (pos_1.x - pos_2.x) +
		(pos_1.y - pos_2.y) * (pos_1.y - pos_2.y));
}

template<typename T>
T Math::min(const Array<T> array)
{
	double result = INFINITY;

	for (int c = 0; c < array.size(); c++)
		if (array[c] < result)
			result = array[c];

	return result;
}

template<typename T>
T Math::min_index(const Array<T> array)
{
	double result = INFINITY;
	int c;

	for (c = 0; c < array.size(); c++)
		if (array[c] < result)
			result = array[c];

	return c;
}

template<typename T>
T Math::max(const Array<T> array)
{
	T result = 0.0;

	for (int c = 0; c < array.size(); c++)
		if (array[c] > result)
			result = array[c];

	return result;
}

template<typename T>
T Math::max_index(const Array<T> array)
{
	double result = 0.0;
	int c;

	for (c = 0; c < array.size(); c++)
		if (array[c] > result)
			result = array[c];

	return c;
}

template<typename T>
T Math::closest_to(T value, T a, T b)
{
	if (distance(value, a) < distance(value, b))
		return a;
	else
		return b;
}

template<typename T>
T Math::closest_to(T value, const Array<T> array)
{
	Array<T> distances;

	for (int c = 0; c < array.size(); c++)
		distances.push_back(distance(value, array[c]));

	return min(distances);
}

template<typename T>
T Math::closest_to_index(T value, const Array<T> array)
{
	Array<T> distances;

	for (int c = 0; c < array.size(); c++)
		distances.push_back(distance(value, array[c]));

	return min_index(distances);
}

#endif