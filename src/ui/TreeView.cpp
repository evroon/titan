#include "TreeView.h"

#include "Canvas.h"
#include "graphics/Renderer.h"

#include "input/Keyboard.h"

#include "Slider.h"


TreeElement::TreeElement(const String &p_text): TreeElement(nullptr)
{
	text = p_text;
}

TreeElement::TreeElement(TreeView* p_treeview)
{
	treeview = p_treeview;
	update_init_expanded();

	text = "";
	depth = 0;
	index = 0;
	parent = nullptr;
	next = nullptr;
	childs = nullptr;
	icon = nullptr;
	text = "";
	secondary_text = "";
	area = rect2();
	visible = true;
	selected = false;
}

TreeElement::~TreeElement()
{
	clear();

	if (parent)
		parent->remove_child(this);

	if (treeview && treeview->roots.contains(this))
		treeview->roots.clear(this);
}

void TreeElement::push_child(TreeElement * p_child)
{
	TreeElement *last = 0;
	TreeElement *c = childs;

	while (c)
	{
		last = c;
		c = c->next;
	}

	if (last)
		last->next = p_child;
	else
		childs = p_child;

	p_child->parent = this;
	p_child->depth = depth + 1;

	update_init_expanded();
}

void TreeElement::remove_child(TreeElement * p_child)
{
	TreeElement **c = &childs;

	while (*c)
	{
		if ((*c) == p_child)
		{
			TreeElement *aux = *c;

			*c = (*c)->next;

			aux->parent = NULL;
			return;
		}

		c = &(*c)->next;
	}
}

void TreeElement::clear()
{
	TreeElement* c = childs;

	while (c)
	{
		TreeElement* aux = c;
		c = c->next;
		aux->parent = NULL;
		delete aux;
	}

	childs = NULL;
}

void TreeElement::bind_treeview(TreeView *p_textbox)
{
	treeview = p_textbox;
}

void TreeElement::update_init_expanded()
{
	if (treeview)
		expanded = depth < treeview->get_init_expanded_depth();
	else
		expanded = true;
}

void TreeElement::set_index(int p_index)
{
	index = p_index;
}

int TreeElement::get_index()
{
	return index;
}

rect2 TreeElement::get_area() const
{
	return area;
}

void TreeElement::set_area(const rect2 & p_area)
{
	area = p_area;
}

bool TreeElement::is_in_area(const vec2 & p_pos)
{
	return visible && area.is_in_box(p_pos);
}

String TreeElement::get_text() const
{
	return text;
}

void TreeElement::set_text(const String & p_text)
{
	text = p_text;
}

Texture2D * TreeElement::get_icon() const
{
	return icon;
}

void TreeElement::set_icon(Texture2D * p_icon)
{
	icon = p_icon;
}

void TreeElement::set_visible(bool p_visible)
{
	visible = p_visible;
}

bool TreeElement::get_visible() const
{
	return visible;
}

float TreeElement::get_height() const
{
	float h = get_item_height();

	if (!expanded)
		return h;

	TreeElement* current = childs;

	while (current)
	{
		h += current->get_height();

		current = current->next;
	}

	return h;
}

float TreeElement::get_item_height() const
{
	return treeview->get_font()->get_height() + 6;
}

int TreeElement::get_index(float x) const
{
	float offset = x - area.get_left();

	return treeview->get_font()->get_index(text, offset);
}
float TreeElement::get_position_x(int index) const
{
	float origin_x = area.get_left();
	return (origin_x + (float)treeview->get_font()->get_width(text.substr(0, index)));
}

void TreeElement::set_expanded(bool p_expanded)
{
	expanded = p_expanded;
	treeview->position_items();
}

bool TreeElement::get_expanded() const
{
	return expanded;
}

bool TreeElement::search(const String& p_text)
{
	TreeElement* current = childs;
	bool v = false;

	while (current)
	{
		if (current->search(p_text))
			v = true;

		current = current->next;
	}

	if (text.contains(p_text))
		v = true;

	set_visible(v);
	expanded = true;

	return v;
}

void TreeElement::clear_search()
{
	TreeElement* current = childs;

	while (current)
	{
		current->clear_search();
		current = current->next;
	}
	visible = true;
	update_init_expanded();
}

TreeElement* TreeElement::get_item(const String& p_name)
{
	TreeElement* current = childs;

	if (text == p_name)
		return this;

	while (current)
	{
		if (current->get_item(p_name))
			return current;

		current = current->next;
	}

	return nullptr;
}

float TreeElement::position(float p_top)
{
	float left = treeview->get_area().get_left();
	float right = treeview->get_area().get_right();
	float top = p_top;
	float bottom = top - get_item_height();

	area = rect2(left, right, top, bottom);

	TreeElement* next = get_next_visible();

	if (next)
		return next->position(bottom);

	return bottom;
}

bool TreeElement::is_selected() const
{
	return selected;
}

TreeElement* TreeElement::get_parent() const
{
	return parent;
}

TreeElement * TreeElement::get_next() const
{
	return next;
}

TreeElement * TreeElement::get_prev()
{
	if (!parent)
	{
		if (treeview->roots[0] == this)
			return nullptr;
		else
			return treeview->roots[treeview->roots.getindex(this) - 1];
	}

	if (parent->childs == this)
		return nullptr;

	TreeElement *prev = parent->childs;

	while (prev && prev->next != this)
		prev = prev->next;

	return prev;
}

TreeElement* TreeElement::get_next_visible()
{
	TreeElement *current = this;
	bool cont = false;

	if (current->expanded && current->childs && current->visible)
	{
		current = current->childs;

		while (current->next && !current->visible)
			current = current->next;

		if (current->visible)
			return current;
	}

	// could not find appropiate child, search next
	current = this;

	while (current->next && (!current->visible || current == this))
		current = current->next;

	if (current != this && current->visible)
		return current;

	// could not find appropiate next, search parents
	current = this;

	while (true)
	{
		current = current->parent;

		if (current && current->next)
		{
			TreeElement* e = current;

			while (current->next && (!current->visible || current == e))
				current = current->next;

			if (current->visible)
				return current;
		}

		if (!current)
			return nullptr;
	}

	return nullptr;
}

TreeElement * TreeElement::get_prev_visible()
{
	TreeElement *current = this;
	TreeElement *prev = current->get_prev();

	if (!prev)
		current = current->parent;
	else
	{
		current = prev;

		while (current->expanded && current->childs)
		{
			current = current->childs;

			while (current->next)
				current = current->next;
		}
	}

	return current;
}


//=========================================================================
//TreeView
//=========================================================================

TreeView::TreeView()
{
	left_margin = 4;
	right_margin = 4;
	top_margin = 3;
	internal_left_margin = 4;
	secondary_margin = 200;
	extra_space = 100;
	depth_offset = 20.0f;

	background_color = TO_RGB(50);
	selection_color = TO_RGB(170);
	item_numbers_color = TO_RGB(200);

	slider = nullptr;
	selected = nullptr;
	highlighted = nullptr;

	font = CanvasData::get_singleton()->get_default_theme()->get_font();
	collapsed_tex = CONTENT->LoadFontAwesomeIcon("solid/plus-square", vec2i(13));
	expanded_tex = CONTENT->LoadFontAwesomeIcon("solid/minus-square", vec2i(13));

	scroll_offset = 0.0f;
	search_text = "";
	
	box = rect2();
	selection_box = rect2();

	roots = Vector<TreeElement>();

	item_numbers_enabled = false;
	set_init_expanded(false);
}

TreeView::~TreeView()
{

}

vec2 TreeView::get_required_size() const
{
	return vec2(100);
}

void TreeView::handle_event(UIEvent *ui_event)
{
	switch (ui_event->type)
	{
	case UIEvent::MOUSE_ENTER:

		break;

	case UIEvent::MOUSE_EXIT:

		highlighted = NULL;

		break;

	case UIEvent::FOCUS_START:

		break;

	case UIEvent::FOCUS_LOSE:

		break;

	case UIEvent::MOUSE_SCROLL:

		if (slider)
		{
			if (ui_event->scroll_type == Event::SCROLL_DOWN)
				slider->step_down();
			else
				slider->step_up();
		}

		break;

	case UIEvent::MOUSE_HOVER:

		highlighted = get_item(ui_event->pos);
		update();

		break;

	case UIEvent::MOUSE_PRESS:

		if (ui_event->press_type == UIEvent::DOWN)
		{
			TreeElement* elm = get_item(ui_event->pos);

			if (elm)
			{
				float offset = elm->area.get_left() + elm->depth * depth_offset + 4;

				rect2 collapse_icon = rect2(offset, offset + 12, elm->area.get_top() - 1, elm->area.get_bottom() + 1);

				if (ui_event->button_type == Mouse::LEFT && collapse_icon.is_in_box(ui_event->pos))
					elm->set_expanded(!elm->expanded);
				else
					set_selected(elm);

				if (ui_event->button_type == Mouse::RIGHT)
				{
					emit_signal("right_clicked", elm);
				}

				break;
			}

			select_none();
		}

		break;

	case UIEvent::MOUSE_DOUBLE_CLICK:

		if (!selected)
			break;

		selected->expanded = !selected->expanded;

		emit_signal("chosen", selected);

		break;

	case UIEvent::KEY_PRESS:

		if (ui_event->press_type == Event::DOWN)
		{
			switch (ui_event->key.key_type)
			{
			case Key::KEY_HOME:

				move_selected_begin();

				break;

			case Key::KEY_END:

				move_selected_end();

				break;

			case Key::KEY_UP:

				move_selected_up();

				break;

			case Key::KEY_DOWN:

				move_selected_down();

				break;

			case Key::KEY_RETURN:

				emit_signal("chosen", selected);

				break;

			case Key::KEY_DELETE:

				//move_selected_right();
				//remove_at_index(caret_pos);
				//move_selected_left();

				break;

			case Key::KEY_A:



				break;
			}
		}

		break;

	case UIEvent::TEXT_INPUT:

		//insert_at_selection(ui_event->text[0]);

		break;
	}
}

void TreeView::position_items()
{
	if (roots.size() > 0)
	{
		float top = area.get_top() - 4 + scroll_offset;

		extra_space = area.get_bottom() - roots[0]->position(top) + 8;

		if (extra_space > 0)
			add_slider();
		else
		{
			extra_space = 0.0f;
			remove_slider();
		}
	}
	update();
}

void TreeView::check_slider_necessity()
{

}

void TreeView::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_READY:
	case NOTIFICATION_TRANSLATED:
	case NOTIFICATION_RESIZED:

		position_items();

		break;

	case NOTIFICATION_DRAW:

		draw_frame(area, TO_RGB(30));

		RENDERER->use_scissor(area);

		bool loop = true;

		for (int c = 0; c < roots.size(); c++)
		{
			TreeElement* item = roots[c];

			while (item)
			{
				rect2 item_area = item->area;

				if (item_area.get_bottom() > area.get_top())
				{
					item = item->get_next_visible();
					continue;
				}

				if (item == highlighted)
					draw_box(item_area, DEFAULT_THEME->get_highlight_color());

				if (item == selected)
					draw_box(item_area, DEFAULT_THEME->get_selection_color());

				if (item_numbers_enabled)
					draw_text(font, c, vec2(item_area.get_left() + 4, item_area.pos.y), item_numbers_color);

				float left = item_area.get_left() + internal_left_margin + depth_offset * item->depth + 4;

				if (item->childs)
				{
					vec2 size = expanded_tex->get_size().y / 4.0f;

					left += size.x;

					if (item->expanded)
						draw_texture(expanded_tex, rect2(vec2(left, item_area.pos.y), collapsed_tex->get_size() / 2.0f), Color::White);
					else
						draw_texture(collapsed_tex, rect2(vec2(left, item_area.pos.y), collapsed_tex->get_size() / 2.0f), Color::White);

					left += size.x + 8;
				}
				else
				{
					left += expanded_tex->get_size().y / 2.0f + 4;
				}
				if (item->get_icon())
				{
					vec2 size = item->get_icon()->get_size() / 2.0f;
					vec2 pos = vec2(left + size.x, item_area.pos.y);

					draw_texture(item->get_icon(), rect2(pos, size), Color::White);
					draw_text(item->get_text(), vec2(pos.x + size.x + 4, pos.y));
				}
				else
					draw_text(item->get_text(), vec2(left, item_area.pos.y));

				float sec_left = item->get_area().get_right() - secondary_margin;

				draw_text(get_font(), item->secondary_text, vec2(sec_left, item_area.pos.y), TO_RGB(vec3i(150, 150, 150)));

				item = item->get_next_visible();
			}
		}

		if (roots.size() == 0)
		{
			float width = get_font()->get_width("No content to display");

			draw_text("No content to display", area.pos - vec2(width / 2.0f, 0));
		}

		RENDERER->stop_scissor();

		if (get_focused())
			draw_frame(DEFAULT_THEME->get_highlight(), area, Color::Green);

		break;
	}
}

TreeElement* TreeView::create_item(TreeElement* p_parent)
{
	TreeElement* item = new TreeElement(this);

	if (p_parent)
		p_parent->push_child(item);
	else
	{
		if (roots.size() > 0)
			roots.getlast()->next = item;

		roots.push_back(item);
		item->parent = NULL;
	}

	position_items();
	return item;
}

void TreeView::push_node(Node* p_node)
{
	clear();
	p_node->connect("children_changed", 
		Connection::create_from_lambda(new V_Method_0([this, p_node](){
			push_node(p_node);
		}))
	);

	TreeElement* root = create_item_from_node(p_node, NULL);
}

TreeElement* TreeView::create_item_from_node(Node* p_node, TreeElement* p_parent)
{
	TreeElement* current = create_item(p_parent);

	current->set_text(p_node->get_name());
	current->secondary_text = p_node->get_type_name();

	for (int c = 0; c < p_node->get_child_count(); c++)
		create_item_from_node(p_node->get_child_by_index(c), current);

	return current;
}

void TreeView::push_back_path(const String& p_path)
{
	Array<String> path = p_path.split('/');

	TreeElement* parent = NULL;
	TreeElement* current = roots.size() == 0 ? NULL : roots[0];

	for (int c = 0; c < path.size(); c++)
	{
		String s = path[c];

		while (current)
		{
			if (current->text == s)
			{
				break;
			}

			current = current->next;
		}

		if (!current)
		{
			current = create_item(parent);
			current->set_text(s);
		}

		parent = current;
		current = current->childs;
	}
}

void TreeView::clear()
{
	roots.clean();

	selected = NULL;
	highlighted = NULL;

	remove_slider();
}

TreeElement* TreeView::get_item(const vec2& p_pos)
{
	if (!area.is_in_box(p_pos) || roots.size() < 1)
		return nullptr;


	TreeElement* current = roots[0];

	while (current)
	{
		if (current->area.is_in_box(p_pos))
			return current;

		current = current->get_next_visible();
	}

	return nullptr;
}

TreeElement* TreeView::get_item(const String& p_name)
{
	for (int c = 0; c < roots.size(); c++)
	{
		TreeElement* e = roots[c]->get_item(p_name);
		if (e)
			return e;
	}
	return nullptr;
}

void TreeView::set_item_numbers_enabled(bool p_value)
{
	item_numbers_enabled = p_value;

	if (item_numbers_enabled)
		left_margin = 15;
	else
		left_margin = 5;

	position_items();
}

void TreeView::add_slider()
{
	if (slider)
		return;

	slider = new Slider;
	add_child(slider);

	slider->set_anchors(ANCHOR_END, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	slider->set_margins(20.0f, 5.0f, 5.0f, 5.0f);

	Connection c;
	c.register_native_method(this, "slider_value_changed");
	slider->connect("value_changed", c);

	right_margin = 24.0f;

	position_items();

	use_scissor = true;
}

void TreeView::remove_slider()
{
	if (!slider)
		return;

	remove_child(slider);
	delete slider;
	slider = NULL;

	right_margin = 4.0f;
	scroll_offset = 0.0f;

	position_items();

	use_scissor = false;
}

void TreeView::set_selected(TreeElement* p_selected)
{
	if (selected == p_selected)
		return;

	selected = p_selected;
	emit_signal("selected", selected);

	make_visible(selected);
	update();
}

TreeElement* TreeView::get_selected() const
{
	return selected;
}

void TreeView::move_selected_begin()
{
	set_selected(roots[0]);
}

void TreeView::move_selected_end()
{
	TreeElement* current = roots.getlast();

	while (true)
	{
		TreeElement* next = current->get_next_visible();

		if (next)
			current = next;
		else
			break;
	}

	set_selected(current);
}

void TreeView::move_selected_up()
{
	if (selected)
	{
		TreeElement* prev = selected->get_prev_visible();

		if (prev)
			set_selected(selected->get_prev_visible());
	}
	else
		move_selected_end();
}

void TreeView::move_selected_down()
{
	if (selected)
	{
		TreeElement* next = selected->get_next_visible();

		if (next)
			set_selected(selected->get_next_visible());
	}
	else
		move_selected_begin();
}
void TreeView::select_none()
{
	set_selected(NULL);
}

void TreeView::make_visible(TreeElement* p_item)
{
	if (!p_item)
		return;

	rect2 item_area = p_item->area;

	float delta = 0.0f;

	if (item_area.get_bottom() < area.get_bottom())
		delta = item_area.get_bottom() - area.get_bottom();

	if (item_area.get_top() > area.get_top())
		delta = item_area.get_top() - area.get_top();

	if (slider && delta != 0.0f)
		slider->set_slider_pos(slider->get_value() - delta / extra_space);
}

void TreeView::set_init_expanded(bool p_init_expanded)
{
	init_expanded_depth = p_init_expanded * INT_MAX;
}

void TreeView::set_init_expanded_depth(int p_init_expanded_depth)
{
	init_expanded_depth = p_init_expanded_depth;
}

int TreeView::get_init_expanded_depth() const
{
	return init_expanded_depth;
}

void TreeView::search(const String& p_src)
{
	if (p_src == "")
	{
		stop_search();
		return;
	}

	for (int c = 0; c < roots.size(); c++)
	{
		roots[c]->search(p_src);
	}

	search_text = p_src;

	position_items();
}

void TreeView::stop_search()
{
	for (int c = 0; c < roots.size(); c++)
	{
		roots[c]->clear_search();
	}
	update();
}

void TreeView::slider_value_changed()
{
	scroll_offset = slider->get_value() * extra_space;

	position_items();
	//update();
}

void TreeView::remove_item(TreeElement* p_item)
{
	if (p_item == selected)
		selected = NULL;

	if (p_item == highlighted)
		highlighted = NULL;

	position_items();
}

#undef CLASSNAME
#define CLASSNAME TreeView

void TreeView::set_font(Font* p_font)
{
	font = p_font;
}

Font* TreeView::get_font() const
{
	return font;
}

void TreeView::bind_methods()
{
	REG_CSTR(0);
	REG_METHOD(slider_value_changed);
	REG_SIGNAL("selected");
	REG_SIGNAL("chosen");
	REG_SIGNAL("right_clicked");
}