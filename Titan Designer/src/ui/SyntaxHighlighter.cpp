#include "SyntaxHighlighter.h"

SyntaxMaster* SyntaxMaster::singleton;

SyntaxMaster::SyntaxMaster()
{
	//Color::FromRGB(vec3i(84, 255, 33))

	glsl.definitions.push_back({ "if ", Color::FromRGB(vec3i(255, 32, 125)) });
	glsl.definitions.push_back({ "else ", Color::FromRGB(vec3i(255, 32, 125)) });
	glsl.definitions.push_back({ "for ", Color::FromRGB(vec3i(255, 32, 125)) });
	glsl.definitions.push_back({ "while ", Color::FromRGB(vec3i(255, 32, 125)) });

	glsl.definitions.push_back({ "uniform ", Color::FromRGB(vec3i(255, 187, 40)) });
	glsl.definitions.push_back({ "in ", Color::FromRGB(vec3i(255, 187, 40)) });
	glsl.definitions.push_back({ "out ", Color::FromRGB(vec3i(255, 187, 40)) });

	glsl.definitions.push_back({ "void", Color::LightBlue });
	glsl.definitions.push_back({ "bool", Color::LightBlue });
	glsl.definitions.push_back({ "int", Color::LightBlue });
	glsl.definitions.push_back({ "float", Color::LightBlue });
	glsl.definitions.push_back({ "vec2", Color::LightBlue });
	glsl.definitions.push_back({ "vec3", Color::LightBlue });
	glsl.definitions.push_back({ "vec4", Color::LightBlue });
	glsl.definitions.push_back({ "mat4", Color::LightBlue });
	glsl.definitions.push_back({ "sampler2D", Color::LightBlue });
	glsl.definitions.push_back({ "sampler3D", Color::LightBlue });
	glsl.definitions.push_back({ "struct", Color::LightBlue });
}


SyntaxMaster::~SyntaxMaster()
{
}

SyntaxMaster* SyntaxMaster::get_singleton()
{
	return singleton;
}

void SyntaxMaster::init()
{
	singleton = new SyntaxMaster;
}
