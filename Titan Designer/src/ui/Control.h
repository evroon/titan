#pragma once

#include "math/Math.h"

#include "core/Node.h"
#include "input/Event.h"

class Texture2D;
class Font;
class Color;
class Canvas;
class Shader;

class Control : public Node
{
	OBJ_DEFINITION(Control, Node);

public:
	Control();

	enum AnchorType
	{
		ANCHOR_BEGIN,
		ANCHOR_END,
		ANCHOR_CENTER,
		ANCHOR_CUSTOM
	};

	enum NotificationType
	{
		NOTIFICATION_READY,
		NOTIFICATION_DRAW,
		NOTIFICATION_UPDATE,
		NOTIFICATION_RESIZED,
		NOTIFICATION_TRANSLATED
	};

	struct DrawCommand
	{
		enum DrawType
		{
			TEXTURE,
			FONT,
			BOX,
			FRAME,
			LINE,
			POLYGON
		} type;

		String text = "";
		rect2 area;
		vec4 bounds;
		vec2 pos;
		Texture2D* tex = NULL;
		Shader* shader = NULL;
		Font* font = NULL;
		Color color;
	};

	void bind_parent(Control *p_parent);
	Control* get_parent() const;

	void init() { update(); notification(NOTIFICATION_READY); }

	virtual vec2 get_required_size() const { return vec2(40); }

	virtual void handle_event(UIEvent *ui_event) { }

	vec2 get_parent_size() const;

	float get_margin(const AnchorType& p_anchor, float p_value, float p_domain, float p_custom = 0.0f) const;

	void set_anchor(int p_index, const AnchorType& p_type);
	void set_margin(int p_index, float p_margin);
	void set_margins(float p_0, float p_1, float p_2, float p_3);

	void set_anchors(const AnchorType & p_0, const AnchorType & p_1, const AnchorType & p_2, const AnchorType & p_3);
	void set_custom_anchors(float p_0, float p_1, float p_2, float p_3);

	void span_parent();

	void set_pos(const vec2& p_pos);
	void set_size(const vec2& p_size);

	vec2 get_pos() const;
	vec2 get_size() const;

	rect2 get_area() const;
	void set_area(const rect2& p_area);

	void float_in_area(const rect2& p_area);

	void set_level(int p_level);
	int get_level() const;

	void set_update_continuoulsy(bool p_update_continuoulsy);
	bool get_update_continuoulsy() const;

	Control* raycast(const vec2 &pos) const;

	void add_child(Node* p_child) override;
	void remove_child(Node* p_child) override;
	
	bool get_focused() const;
	virtual void set_focused(bool value);

	void set_visible(bool p_visible);
	bool get_visible() const;

	bool in_area(const vec2 &pos) const;

	void check_size_changed();
	void flag_size_changed();
	void size_changed();

	void set_tip_description(const String& p_description);
	String get_tip_description() const;

	Font* get_default_font() const;

	Canvas* get_canvas() const;

	void show_context_tip(const vec2& p_pos);

	virtual String get_context_tip(const vec2& p_pos);

	void draw();

	static void bind_methods();

protected:
	virtual void notification(int p_notification) { }

	void draw_texture(Texture2D* p_texture, const rect2& p_area, const Color& p_color, const vec4& p_bounds = vec4(0, 1, 1, 0));
	void draw_text(const String& p_text, const vec2& p_pos);
	void draw_text(Font * p_font, const String & p_text, const vec2 & p_pos, const Color& p_color);
	void draw_text(Font* p_font, const String& p_text, const vec2& p_pos);
	void draw_box(const rect2& p_area, const Color& p_color);
	void draw_box(const rect2& p_area, const Color& p_color, Shader* p_shader);
	void draw_polygon();
	void draw_line(const vec2& p_start, const vec2& p_end, const Color& p_color);
	void draw_frame(Texture2D* p_texture, const rect2& p_area, const Color& p_color);
	void draw_frame(const rect2& p_area, const Color& p_color);
	void draw_bordered_box(const rect2& p_area, const Color& p_border, const Color& p_fill, float edge = 1);

	void draw_highlight(const rect2& p_area, const Color& p_color);

	void update();

	rect2 area;

private:

	void render();

	Array<DrawCommand> draw_commands;

	bool to_be_updated;
	bool update_continuoulsy;

	float margins[4];
	AnchorType anchors[4];
	float custom_anchors[4];

	bool drawing;

	String tip_description;

	int level;

	bool visible;
	bool enabled;
	bool flagged_size_changed;
};