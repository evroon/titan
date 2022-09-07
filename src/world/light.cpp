#include "light.h"

#if 0

Light::Light(Shader* shader, Texture *tex, vec2 pos, double radius)
{
	transform.update(pos, vec2((float)radius));
	//box = new Box(vec3(pos, 0.0f), vec3(WINDOWSIZE.x, WINDOWSIZE.y, 0.0f));
	box->shader = shader;
	this->shader = shader;
	lightmap = tex;
	color = vec4(1.0);
	this->radius = radius;
}

Light::Light(Shader* shader, Texture * tex, vec2 pos, double radius, vec4 color)
{
	transform.update(pos, vec2((float)radius));
	//box = new Box(pos, vec2((float)radius));
	box->shader = shader;
	lightmap = tex;
	this->color = color;
	this->radius = radius;
}


Light::~Light()
{
	delete box;
}

void Light::Draw()
{
	box->get_transform().update();
	shader->bind();
	shader->set_uniform("color", color);
	float rad = (float)radius / WINDOWSIZE.x;
	shader->set_uniform("radius", rad);
	box->set_color(color);
	box->draw();
}

#endif

PointLight::PointLight()
{

}

PointLight::~PointLight()
{
}

#undef CLASSNAME
#define CLASSNAME PointLight

void PointLight::bind_methods()
{
	REG_CSTR(0);
}

//=========================================================================
//DirectionalLight
//=========================================================================

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::~DirectionalLight()
{
}

vec3 DirectionalLight::get_direction() const
{
	return (get_transform().get_model().inverse() * vec4(0.0f, 1.0f, 0.0f, 0.0f)).get_xyz().normalize();
}

#undef CLASSNAME
#define CLASSNAME DirectionalLight

void DirectionalLight::bind_methods()
{
	REG_CSTR(0);
}

//=========================================================================
//ConeLight
//=========================================================================

ConeLight::ConeLight()
{
}

ConeLight::~ConeLight()
{
}

#undef CLASSNAME
#define CLASSNAME ConeLight

void ConeLight::bind_methods()
{
	REG_CSTR(0);
}
