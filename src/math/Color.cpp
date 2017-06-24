#include "Color.h"

Color Color::White	= Color();
Color Color::Black = Color(vec4(0.0f, 0.0f, 0.0f, 1.0f));
Color Color::Red	= Color(vec4(1.0f, 0.0f, 0.0f, 1.0f));
Color Color::Green	= Color(vec4(0.0f, 1.0f, 0.0f, 1.0f));
Color Color::Blue	= Color(vec4(0.0f, 0.0f, 1.0f, 1.0f));
Color Color::LightBlue = Color(vec4(0.0f, 1.0f, 1.0f, 1.0f));
Color Color::Grey = Color(vec4(0.8f, 0.8f, 0.8f, 1.0f));
Color Color::DarkGrey = Color(vec4(0.3f, 0.3f, 0.3f, 1.0f));

String Color::toString() const
{
	return "{ r = " + std::to_string(r) + ", g = " + std::to_string(g) + ", b = " + std::to_string(b) + ", a = " + std::to_string(a) + " }";
}
