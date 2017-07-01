#include "Control.h"

#include "Container.h"
#include "graphics/View.h"
#include "graphics/Renderer.h"
#include "resources/Texture.h"
#include "TChar.h"

#include "core/WindowManager.h"

Control::Control()
{
	for (int c = 0; c < 4; c++)
	{
		anchors[c] = ANCHOR_CENTER;
		margins[c] = 0.0f;
	}
}

void Control::bind_parent(Control *p_parent)
{
	parent = p_parent;
}

Control* Control::get_parent() const
{
	return parent->cast_to_type<Control*>();
}

vec2 Control::get_parent_size() const
{
	if (parent && !parent->is_of_type<Canvas>())
	{
		Control* p = parent->cast_to_type<Control*>();
		return p->area.get_size();
	}
	else
	{
		return ACTIVE_VIEWPORT->get_size();
	}
}

float Control::get_margin(const AnchorType & p_anchor, float p_value, float p_domain, float p_custom) const
{
	switch (p_anchor)
	{
	case ANCHOR_BEGIN:
		return p_value;

	case ANCHOR_END:
		return p_domain - p_value;

	case ANCHOR_CENTER:
		return -(p_domain / 2.0f) + p_value;

	case ANCHOR_CUSTOM:
		return -(p_domain * p_custom) + p_value;
	}

	return 0;
}

void Control::set_anchor(int p_index, const AnchorType& p_type)
{
	anchors[p_index] = p_type;

	size_changed();
}

void Control::set_margin(int p_index, float p_margin)
{
	margins[p_index] = p_margin;

	size_changed();
}

void Control::set_margins(float p_0, float p_1, float p_2, float p_3)
{
	margins[0] = p_0;
	margins[1] = p_1;
	margins[2] = p_2;
	margins[3] = p_3;

	size_changed();
}

void Control::set_anchors(const AnchorType& p_0, const AnchorType& p_1, const AnchorType& p_2, const AnchorType& p_3)
{
	anchors[0] = p_0;
	anchors[1] = p_1;
	anchors[2] = p_2;
	anchors[3] = p_3;

	size_changed();
}

void Control::set_custom_anchors(float p_0, float p_1, float p_2, float p_3)
{
	set_anchors(ANCHOR_CUSTOM, ANCHOR_CUSTOM, ANCHOR_CUSTOM, ANCHOR_CUSTOM);

	custom_anchors[0] = p_0;
	custom_anchors[1] = p_1;
	custom_anchors[2] = p_2;
	custom_anchors[3] = p_3;
}

void Control::span_parent()
{
	set_anchor(0, ANCHOR_BEGIN);
	set_anchor(1, ANCHOR_BEGIN);
	set_anchor(2, ANCHOR_END);
	set_anchor(3, ANCHOR_END);

	set_margins(0.0f, 0.0f, 0.0f, 0.0f);

	size_changed();
}

Font* Control::get_default_font() const
{
	return CanvasData::get_singleton()->get_default_theme()->get_font();
}

Canvas* Control::get_canvas() const
{
	return ACTIVE_CANVAS;
}

void Control::show_context_tip(const vec2& p_pos)
{
	get_canvas()->set_context_tip(get_context_tip(p_pos), p_pos);
}

String Control::get_context_tip(const vec2& p_pos)
{
	String text;

	if (tip_description.length() > 0)
		text = tip_description;
	else
		text = name;

	return text;
}

void Control::set_pos(const vec2& p_pos)
{
	set_area(rect2(p_pos, area.size));
}

void Control::set_size(const vec2& p_size)
{
	set_area(rect2(area.pos, p_size));
}

vec2 Control::get_pos() const
{
	return area.pos;
}

vec2 Control::get_size() const
{
	return area.size;
}

rect2 Control::get_area() const
{
	return area;
}

void Control::set_area(const rect2& p_area)
{
	vec2 parent_pos;
	vec2 parent_size = get_parent_size() * 2.0f;

	if (parent && !parent->is_of_type<Canvas>())
		parent_pos = parent->cast_to_type<Control*>()->get_area().get_bottom_left();
	else
		parent_pos = get_parent_size() * -1.0f;

	vec2 local_pos = p_area.get_bottom_left() - parent_pos;

	for (int c = 0; c < 4; c++)
	{
		margins[c] = get_margin(anchors[c], local_pos[c & 1], parent_size[c & 1], custom_anchors[c]);
	}

	size_changed();
}

void Control::float_in_area(const rect2& p_area)
{
	area = p_area;
}

void Control::set_level(int p_level)
{
	level = p_level;
}

int Control::get_level() const
{
	return level;
}

Control* Control::raycast(const vec2 & pos) const
{
	for (int i = 0; i < children.size(); i++)
	{
		Control *c = children[i]->cast_to_type<Control*>();

		if (c->in_area(pos))
			return c->raycast(pos);
	}

	if (in_area(pos))
		return const_cast<Control*>(this);

	return NULL;
}

bool Control::in_area(const vec2 &pos) const
{
	return area.is_in_box(pos);
}

void Control::add_child(Node * p_child)
{
	Node::add_child(p_child);

	p_child->cast_to_type<Control*>()->init();
}

void Control::remove_child(Node * p_child)
{
	Node::remove_child(p_child);
}

bool Control::get_focused() const
{
	return ACTIVE_CANVAS->get_focused() == this;
}

void Control::set_focused(bool value)
{
	if (value)
		ACTIVE_CANVAS->focus(this);
}

void Control::set_visible(bool p_visible)
{
	visible = p_visible;
}

bool Control::get_visible() const
{
	return visible;
}

void Control::size_changed()
{
	vec2 minimum_size = get_required_size();
	vec2 bottom_left, top_right, parent_size;
	vec4 points;



	if (parent && !parent->is_of_type<Canvas>())
	{
		Control* p = parent->cast_to_type<Control*>();
		bottom_left = p->area.get_bottom_left();
		top_right = p->area.get_upper_right();
	}
	else
	{
		bottom_left = ACTIVE_VIEWPORT->get_size() * vec2(-1);
		top_right = ACTIVE_VIEWPORT->get_size();
	}

	parent_size = top_right - bottom_left;
	points = vec4(bottom_left, top_right);

	float pos[4];

	for (int i = 0; i < 4; i++)
	{
		float domain = parent_size[i & 1];

		switch (anchors[i])
		{
		case ANCHOR_BEGIN:

			pos[i] = margins[i];
			break;

		case ANCHOR_END:

			pos[i] = domain - margins[i];
			break;

		case ANCHOR_CENTER:

			pos[i] = domain / 2.0f + margins[i];
			break;

		case ANCHOR_CUSTOM:

			pos[i] = domain * custom_anchors[i] + margins[i];
			break;
		}

		pos[i] += bottom_left[i & 1];
	}

	if (pos[2] - pos[0] < minimum_size.x)
		pos[2] = pos[0] + minimum_size.x;

	if (pos[3] - pos[1] < minimum_size.y)
		pos[3] = pos[1] + minimum_size.y;

	rect2 new_area = rect2(pos[0], pos[2], pos[3], pos[1]);

	if (pos[2] > top_right.x)
		new_area.pos.x -= pos[2] - top_right.x;

	if (pos[3] > top_right.y)
		new_area.pos.y -= pos[3] - top_right.y;

	//notificate

	bool pos_changed = area.pos != new_area.pos;
	bool size_changed = area.size != new_area.size;

	area = new_area;

	if (pos_changed)
		notification(NOTIFICATION_TRANSLATED);

	if (size_changed)
		notification(NOTIFICATION_RESIZED);
	
	draw();

	for (Node* n : children)
		n->cast_to_type<Control*>()->size_changed();
}

void Control::set_tip_description(const String & p_description)
{
	tip_description = p_description;
}

String Control::get_tip_description() const
{
	return tip_description;
}

void Control::draw()
{
	drawing = true;

	notification(NOTIFICATION_DRAW);

	drawing = false;

	for (Node* n : children)
		n->cast_to_type<Control*>()->draw();
}

#define CHECK_DRAWING \
if (!drawing)\
{\
	T_ERROR("Can only draw inside NOTIFICATION_DRAW");\
	return;\
}

void Control::draw_texture(Texture2D* p_texture, const rect2& p_area, const Color& p_color, const vec4& p_bounds)
{
	CHECK_DRAWING

	if (!p_texture)
	{
		T_ERROR("p_texture is NULL");
		return;
	}

	SimpleMesh* mesh = MeshHandler::get_singleton()->get_plane();
	Shader* shader = CanvasData::get_singleton()->get_default_shader();
	Transform transform = Transform(p_area.pos, p_area.size);

	shader->Bind();
	shader->setUniform("texbounds", p_bounds);
	shader->setUniform("color", p_color);
	shader->setUniform("texture_enabled", true);

	p_texture->Bind(0);

	mat4 final = RENDERER->get_final_matrix() * transform.get_model();
	shader->setUniform("model", final);
	mesh->draw();
}

void Control::draw_text(const String& p_text, const vec2& p_pos)
{
	draw_text(get_default_font(), p_text, p_pos);
}

void Control::draw_text(Font * p_font, const String & p_text, const vec2 & p_pos)
{
	draw_text(p_font, p_text, p_pos, Color::White);
}

void Control::draw_text(Font* p_font, const String& p_text, const vec2& p_pos, const Color& p_color)
{
	CHECK_DRAWING

	if (!p_font)
	{
		T_ERROR("p_font is NULL");
		return;
	}

	float offset = 0.0f;

	vec2i parent_size = WINDOWSIZE;
	vec2 delta;

	if (parent_size.x % 2 != 0)
		if (to_int(p_pos.x) % 2 != 0)
			delta.x = 1;

	if (parent_size.y % 2 != 0)
		if (to_int(p_pos.y) % 2 != 0)
			delta.y = 1;

	vec2 pos = vec2(Math::floor(p_pos.x), Math::floor(p_pos.y)) + delta;

	Texture2D* tex = p_font->get_renderer()->get_texture();
	SimpleMesh* mesh = MeshHandler::get_singleton()->get_plane();
	Shader* shader = CanvasData::get_singleton()->get_default_shader();

	shader->Bind();
	shader->setUniform("color", p_color);
	shader->setUniform("texture_enabled", true);

	tex->Bind(0);

	for (int i = 0; i < p_text.size(); i++)
	{
		Char c = p_text[i];

		if (c == ' ')
		{
			offset += p_font->get_renderer()->get_space_offset();
			continue;
		}
		if (c == '\t')
		{
			offset += p_font->get_renderer()->get_tab_offset();
			continue;
		}
		if (c < 33 || c > 126)
		{
			offset += p_font->get_renderer()->get_space_offset();
			continue;
		}

		vec4 bounds = p_font->get_renderer()->get_bounds(c);
		vec4 b = vec4(bounds.x / tex->size.x, bounds.y / tex->size.x, 1, 0);

		float delta = bounds.y - bounds.x;

		Transform transform = Transform(vec2(pos.x + offset + delta / 2.0f, pos.y), vec2(delta, tex->size.y) / 2.0f);

		shader->setUniform("model", RENDERER->get_final_matrix() * transform.get_model());
		shader->setUniform("texbounds", b);

		mesh->draw();

		offset += delta;
	}
}

void Control::draw_box(const rect2& p_area, const Color& p_color)
{
	CHECK_DRAWING

	SimpleMesh* mesh = MeshHandler::get_singleton()->get_plane();
	Shader* shader = CanvasData::get_singleton()->get_default_shader();
	Transform transform = Transform(p_area.pos, p_area.size);

	shader->Bind();
	shader->setUniform("color", p_color);
	shader->setUniform("texture_enabled", false);

	glBindTexture(GL_TEXTURE_2D, 0);

	shader->setUniform("model", RENDERER->get_final_matrix() * transform.get_model());
	mesh->draw();
}

void Control::draw_polygon()
{
	CHECK_DRAWING
}

void Control::draw_line(const vec2& p_start, const vec2& p_end, const Color& p_color)
{
	CHECK_DRAWING

	SimpleMesh* mesh = MeshHandler::get_singleton()->get_line();
	Shader* shader = CanvasData::get_singleton()->get_default_shader();

	//"center of mass"-like coordinates
	vec2 delta = p_end - p_start;
	vec2 center = (p_end + p_start) / 2.0f;

	float angle = delta.angle(vec2(0, -1.0f));
	float length = delta.length() / 2.0f;

	Transform transform = Transform(center, vec2(1.0f, length), angle);

	shader->Bind();
	shader->setUniform("color", p_color);
	shader->setUniform("texture_enabled", false);

	glBindTexture(GL_TEXTURE_2D, 0);

	shader->setUniform("model", RENDERER->get_final_matrix() * transform.get_model());
	mesh->bind();
	mesh->draw();

	mesh = MeshHandler::get_singleton()->get_plane();
	mesh->bind();

}

void Control::draw_frame(Texture2D* p_texture, const rect2& p_area, const Color& p_color)
{
	CHECK_DRAWING

	if (!p_texture)
	{
		T_ERROR("p_texture is NULL");
		return;
	}

	vec2 tex_size = p_texture->size;
	vec2 size = p_area.size;

	vec2 pos = p_area.get_bottom_left();
	vec2 extension = { size.x * 2 - tex_size.x * 2 / 3, size.y * 2 - tex_size.x * 2 / 3 };

	Array<float> h_off = { 0, tex_size.x / 3, 2 * tex_size.x / 3, tex_size.x };
	Array<float> v_off = { 0, tex_size.y / 3, 2 * tex_size.y / 3, tex_size.y };
	Array<float> h_pos = { pos.x, pos.x + h_off[1], pos.x + extension.x + h_off[1], pos.x + extension.x + h_off[2] };
	Array<float> v_pos = { pos.y, pos.y + v_off[1], pos.y + extension.y + v_off[1], pos.y + extension.y + v_off[2] };

	SimpleMesh* mesh = MeshHandler::get_singleton()->get_plane();
	Shader* shader = CanvasData::get_singleton()->get_default_shader();
	Transform transform;

	shader->Bind();
	shader->setUniform("color", p_color);
	shader->setUniform("texture_enabled", true);

	p_texture->Bind(0);

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			vec2 horiz_bounds = vec2(h_off[x], h_off[x + 1]);
			vec2 vert_bounds = vec2(v_off[y], v_off[y + 1]);

			rect2 new_area = rect2(h_pos[x], h_pos[x + 1], v_pos[y + 1], v_pos[y]);

			vec4 b = vec4(horiz_bounds, vert_bounds);
			vec4 bounds = vec4(vec2(b.x, b.y) / p_texture->size.x, vec2(b.z, b.w) / p_texture->size.y);

			transform = Transform(new_area.pos, new_area.size);

			shader->setUniform("texbounds", bounds);
			shader->setUniform("model", RENDERER->get_final_matrix() * transform.get_model());

			mesh->draw();
		}
	}
}

void Control::draw_frame(const rect2& p_area, const Color& p_color)
{
	draw_frame(CanvasData::get_singleton()->get_default_theme()->get_frame(), p_area, p_color);
}

void Control::draw_bordered_box(const rect2 & p_area, const Color & p_border, const Color & p_fill, float edge)
{
	draw_box(p_area, p_border);
	rect2 inside = p_area.crop(edge);
	draw_box(inside, p_fill);
}

void Control::draw_highlight(const rect2& p_area, const Color& p_color)
{
	draw_frame(CanvasData::get_singleton()->get_default_theme()->get_highlight(), p_area, p_color);
}

void Control::update()
{
	if (!to_be_updated && ACTIVE_CANVAS)
		ACTIVE_CANVAS->schedule_update(this);

	size_changed();

	to_be_updated = true;
}


#undef CLASSNAME
#define CLASSNAME Control

void Control::bind_methods()
{
	REG_PROPERTY(pos);
	REG_PROPERTY(size);
	REG_PROPERTY(focused);
}