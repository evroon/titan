#include "Color.h"

#include "math/Math.h"

Color Color::White	= Color();
Color Color::Black = Color(vec4(0.0f, 0.0f, 0.0f, 1.0f));
Color Color::Red	= Color(vec4(1.0f, 0.0f, 0.0f, 1.0f));
Color Color::Green	= Color(vec4(0.0f, 1.0f, 0.0f, 1.0f));
Color Color::Blue	= Color(vec4(0.0f, 0.0f, 1.0f, 1.0f));
Color Color::LightBlue = Color(vec4(0.0f, 1.0f, 1.0f, 1.0f));
Color Color::Grey = Color(vec4(0.8f, 0.8f, 0.8f, 1.0f));
Color Color::DarkGrey = Color(vec4(0.3f, 0.3f, 0.3f, 1.0f));

void Color::from_hsv(vec3 val)
{
	float h = val.x;
	float s = val.y;
	float v = val.z;

	float q = h * 6.0f;

	float c = v * s;
	float x = c * (1.0f - Math::abs(static_cast<int>(q) % 2 + q - static_cast<int>(q) - 1.0f));

	float m = v - c;

	switch (static_cast<int>(q))
	{
	case 0:
		r = c;		g = x;		b = 0.0f;
		break;
	case 1:
		r = x;		g = c;		b = 0.0f;
		break;
	case 2:
		r = 0.0f;	g = c;		b = x;
		break;
	case 3:
		r = 0.0f;	g = x;		b = c;
		break;
	case 4:
		r = x;		g = 0.0f;	b = c;
		break;
	case 5:
		r = c;		g = 0.0f;	b = x;
		break;
	}

	r += m;
	g += m;
	b += m;
}

Color Color::from_hsv(vec3i val)
{
	Color c;
	c.from_hsv(vec3(static_cast<float>(val.x), static_cast<float>(val.y), static_cast<float>(val.z)) / 256.0f);
	return c;
}

vec3 Color::get_hsv() const
{
	vec3 in = rgb;
	vec3 out;
	double min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min  < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max  > in.b ? max : in.b;

	out.z = max;
	delta = max - min;

	if (delta < 0.00001)
	{
		out.y = 0;
		out.x = 0;
		return out;
	}

	if (max > 0.0)
		out.y = (delta / max);
	else
	{
		out.y = 0.0;
		out.x = NAN;
		return out;
	}

	if (in.r >= max)
		out.x = (in.g - in.b) / delta;
	else
	{
		if (in.g >= max)
			out.x = 2.0 + (in.b - in.r) / delta;
		else
			out.x = 4.0 + (in.r - in.g) / delta;
	}

	out.x *= 60.0;

	if (out.x < 0.0)
		out.x += 360.0;

	out.x *= PI / 180.0f;

	return out;
}

String Color::toString() const
{
	return "{ r = " + std::to_string(r) + ", g = " + std::to_string(g) + ", b = " + std::to_string(b) + ", a = " + std::to_string(a) + " }";
}
