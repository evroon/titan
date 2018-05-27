#include "ResourceField.h"

#include "TextField.h"
#include "TextButton.h"
#include "Dialog.h"

#include "graphics/View.h"
#include "editor/EditorApp.h"

//=========================================================================
//ColorField
//=========================================================================

ColorField::ColorField()
{
	init();
}

ColorField::~ColorField()
{
}

ColorField::ColorField(Color& p_color) : PropertyControl(p_color)
{
	init();
}

ColorField::ColorField(const Variable& p_variable) : PropertyControl(p_variable)
{
	init();
}

void ColorField::init()
{
	textbutton = new TextButton("...");
	add_child(textbutton);

	textbutton->set_margins(30, 4, 4, 4);
	textbutton->set_anchors(ANCHOR_END, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);

	textbutton->connect("clicked", this, "show_dialog");
}

void ColorField::color_chosen(const Color & p_color)
{
	value_set(p_color);
}

vec2 ColorField::get_required_size() const
{
	return vec2(100, textbutton->get_required_size().y);
}

void ColorField::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		rect2 color_area = area.crop(0, 40, 0, 0);

		draw_box(color_area, get_value());

		break;
	}
}

void ColorField::show_dialog()
{
	color_dialog = new ColorPickDialog(get_value().operator Color &());
	color_dialog->show();
	color_dialog->connect("color_chosen", this, "color_chosen");
}

#undef CLASSNAME
#define CLASSNAME ColorField

void ColorField::bind_methods()
{
	REG_CSTR(0);
	REG_CSTR_OVRLD_1(Color);

	REG_METHOD(show_dialog);
	REG_METHOD(color_chosen);

	REG_SIGNAL("color_chosen");
}

//=========================================================================
//Texture2DField
//=========================================================================

Texture2DField::Texture2DField()
{
	init();
}

Texture2DField::Texture2DField(Texture2D* p_texture) : Texture2DField()
{
	texture = p_texture;
	init();
}

Texture2DField::Texture2DField(const Variable & p_variable) : PropertyControl(p_variable)
{
	init();
}

Texture2DField::~Texture2DField()
{
}

void Texture2DField::init()
{
	load_button = new TextButton("Load");
	add_child(load_button);
	
	load_button->set_margins(100, 0, 40, 0);
	load_button->set_anchors(ANCHOR_END, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	load_button->connect("clicked", this, "load_texture");

	open_button = new TextButton("...");
	open_button->connect("clicked", this, "open_button_clicked");
	add_child(open_button);

	open_button->set_margins(30, 0, 4, 0);
	open_button->set_anchors(ANCHOR_END, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);

	file_dialog = new FileDialog(CONTENT->get_assets_dir() + "/");

}

vec2 Texture2DField::get_required_size() const
{
	return vec2(100, 40);
}

void Texture2DField::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		if (!get_value())
			return;

		rect2 color_area = rect2(area.get_left(), area.get_left() + 40, area.get_top(), area.get_bottom());

		draw_texture(get_value(), color_area, Color::White);

		break;
	}
}

void Texture2DField::load_texture()
{
	file_dialog->show();
	file_dialog->connect("file_chosen", this, "file_chosen");
}

void Texture2DField::file_chosen(const String& p_path)
{
	value_set(CONTENT->LoadTexture(p_path));
}

#undef CLASSNAME
#define CLASSNAME Texture2DField

void Texture2DField::bind_methods()
{
	REG_CSTR(0);

	REG_METHOD(load_texture);
	REG_METHOD(file_chosen);
}

//=========================================================================
//ObjectField
//=========================================================================

ObjectField::ObjectField()
{
	init();
}

ObjectField::ObjectField(Object* p_object) : PropertyControl(p_object)
{
	init();
	object = p_object;

	Node* node = reinterpret_cast<Node*>(object.operator Object *());

	if (node)
	{
		textfield->set_text(node->get_name());
	}
	else
	{
		Resource* resource = reinterpret_cast<Resource*>(p_object);

		if (resource)
			textfield->set_text(File(resource->get_file()).get_name());
	}
}

ObjectField::ObjectField(const Variable& p_variable) : PropertyControl(p_variable)
{
	init();

	Variable var = Variable(p_variable);
	object = var.get().operator Object *();

	Node* node = dynamic_cast<Node*>(object.operator Object *());

	if (node)
	{
		textfield->set_text(node->get_name());
	}
	else
	{
		Resource* resource = dynamic_cast<Resource*>(object.operator Object *());

		if (resource)
			textfield->set_text(File(resource->get_file()).get_name());
	}
}

ObjectField::~ObjectField()
{

}

void ObjectField::init()
{	
	textfield = new TextField;
	load_button = new TextButton("Load");
	
	add_child(textfield);
	add_child(load_button);

	load_button->set_margins(100, 0, 40, 0);
	load_button->set_anchors(ANCHOR_END, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	load_button->connect("clicked", this, "load_button_clicked");

	textfield->set_margins(0, 0, 104, 0);
	textfield->set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);

	String text = "New";
	if (get_value().isdef())
		text = "Open";

	open_button = new TextButton(text);
	open_button->connect("clicked", this, "open_button_clicked");
	add_child(open_button);

	open_button->set_margins(30, 0, 4, 0);
	open_button->set_anchors(ANCHOR_END, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);

}

void ObjectField::open_button_clicked()
{
	VariantType type = PropertyControl::get_property_type();
	Variant v = get_value();

	if (!v)
	{
		Variant result;

		if (MMASTER->constructor_exists(type, 0))
			result = reinterpret_cast<CSTR_0*>(MMASTER->get_constructor(type, 0))->operator() ();

		value_set(result);

		if (result.operator Object*()->derives_from_type<Resource*>())
			textfield->set_text(File(result.operator Resource*()->get_file()).get_name());
		else if(result.operator Object*()->derives_from_type<Node*>())
			textfield->set_text(result.operator Node*()->get_name());

		open_button->set_text("Open");
		return;
	}

	if (v.operator Object*()->derives_from_type<Resource*>())
		EDITOR_APP->open_file(File(v.operator Resource*()->get_file()).get_absolute_path());
	else if (v.operator Object*()->derives_from_type<Node*>())
		textfield->set_text(v.operator Node*()->get_name());

	//emit_signal("set_property", object.operator Object *()); 
}

void ObjectField::load_button_clicked()
{
	file_dialog = new FileDialog(CONTENT->get_assets_dir() + "/");
	add_child(file_dialog);
	file_dialog->connect("file_chosen", this, "file_chosen");
	file_dialog->show();
}

void ObjectField::file_chosen(const String& p_path)
{
	value_set(CONTENT->Load(p_path));
	textfield->set_text(File(p_path).get_name());
	remove_child(file_dialog);
}

vec2 ObjectField::get_required_size() const
{
	return vec2(100, textfield->get_required_size().y);
}

#undef CLASSNAME
#define CLASSNAME ObjectField

void ObjectField::bind_methods()
{
	REG_METHOD(open_button_clicked);
	REG_METHOD(load_button_clicked);
	REG_METHOD(file_chosen);

	REG_SIGNAL("set_property");
}

//=========================================================================
//ShaderField
//=========================================================================


ShaderField::ShaderField()
{
}

ShaderField::ShaderField(Shader* p_shader)
{
}

ShaderField::ShaderField(const Variable& p_variable)
{
}

ShaderField::~ShaderField()
{
}

void ShaderField::init()
{
}

void ShaderField::open_button_clicked()
{
}

vec2 ShaderField::get_required_size() const
{
	return vec2();
}

void ShaderField::bind_methods()
{
}
