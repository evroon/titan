#include "Vec3Field.h"

Vec3Field::Vec3Field() : Vec3Field(vec3())
{

}

Vec3Field::Vec3Field(const vec3 &p_vec3)
{
	x.label.set_text(String(p_vec3.x));
	y.label.set_text(String(p_vec3.y));
	z.label.set_text(String(p_vec3.z));
	
	container.add_child(&x);
	container.add_child(&y);
	container.add_child(&z);
}

vec2 Vec3Field::get_required_size() const
{
	return x.get_required_size();
}

void Vec3Field::handle_event(UIEvent *ui_event)
{
	
}

void Vec3Field::draw()
{
	container.draw();
}