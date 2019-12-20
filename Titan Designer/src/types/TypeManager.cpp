#include "TypeManager.h"

#include "core/TMessage.h"
#include "core/variant/VariantType.h"

TypeManager *TypeManager::singleton;


TypeManager::TypeManager()
{
}


TypeManager::~TypeManager()
{
}

void TypeManager::add_type(const StringName &name)
{
	types[name] = name;
}

VariantType TypeManager::get_type(const StringName &name)
{
	return types[name];
}

VariantType TypeManager::get_type(void *ptr)
{
	return get_type(get_name(ptr));
}

StringName TypeManager::get_name(void *ptr)
{
	return names[ptr];
}

ObjectType TypeManager::get_object_type(const StringName &name)
{
	if (type_exists(name))
		return object_types[name];
	else
	{
		T_ERROR("type does not exist: " + std::string(name.get_source()));
		return {};
	}
}

bool TypeManager::type_exists(const StringName &name) const
{
	return types.contains(name);
}

void TypeManager::set_object_type(const ObjectType& p_object_type)
{
	object_types[p_object_type.name] = p_object_type;
	types[p_object_type.name] = p_object_type.name;
	names[p_object_type.ptr] = p_object_type.name;
}

TypeManager* TypeManager::get_singleton()
{
	return singleton;
}

#include "world/Sprite.h"
#include "world/World.h"
#include "world/Terrain.h"
#include "world/Sky.h"
#include "world/Model.h"
#include "world/Light.h"
#include "world/Environment.h"

#include "Scriptable.h"
#include "core/titanscript/TitanScript.h"
#include "input/Keyboard.h"
#include "core/Time.h"
#include "ui/Label.h"
#include "ui/TextEditorTab.h"
#include "ui/ImageButton.h"
#include "ui/ListView.h"
#include "ui/TileView.h"
#include "ui/TreeView.h"
#include "ui/ToolBar.h"
#include "graphics/View.h"
#include "ui/Dialog.h"
#include "ui/ConsoleTab.h"
#include "ui/ExplorerTab.h"
#include "ui/ContentTab.h"
#include "editor/EditorApp.h"
#include "ui/PropertyTab.h"
#include "ui/PropertyView.h"
#include "ui/WorldView.h"
#include "ui/ResourceField.h"
#include "ui/NumberField.h"
#include "ui/VectorField.h"
#include "physics/RigidBody.h"
#include "physics/CollisionShape.h"
#include "math/TransformComponent.h"
#include "graphics/RenderComponent.h"
#include "ui/ComboBox.h"
#include "ui/GamePreviewTab.h"
#include "ui/ToolTab.h"
#include "ui/Toggle.h"

void TypeManager::init()
{
	singleton = new TypeManager;

	WorldObject::init_type();
	PointLight::init_type();
	DirectionalLight::init_type();
	ConeLight::init_type();
	Sprite::init_type();
	World::init_type();
	Camera::init_type();
	Terrain::init_type();
	TerrainBrush::init_type();
	Water::init_type();
	Sky::init_type();
	Clouds::init_type();
	Vegetation::init_type();
	Model::init_type();
	Mesh::init_type();
	Material::init_type();
	Environment::init_type();

	Scriptable::init_type();
	TitanScript::init_type();
	Mouse::init_type();
	Event::init_type();
	InputEvent::init_type();
	UIEvent::init_type();
	CollisionEvent::init_type();
	Keyboard::init_type();
	Key::init_type();
	Time::init_type();
	RigidBody2D::init_type();
	RigidBody3D::init_type();
	CollisionShape2D::init_type();
	CircleShape2D::init_type();
	BoxShape2D::init_type();
	CollisionShape3D::init_type();
	TransformComponent::init_type();
	RenderComponent::init_type();
	Component::init_type();
	Viewport::init_type();
	EditorViewport::init_type();
	ContentManager::init_type();
	EditorApp::init_type();
	Project::init_type();
	Scene::init_type();
	Layer::init_type();

	//Nodes
	Node::init_type();
	Canvas::init_type();
	Label::init_type();
	Button::init_type();
	LabelButton::init_type();
	ImageButton::init_type();
	Toggle::init_type();
	ToggleStrip::init_type();
	Tab::init_type();
	TextEditorTab::init_type();
	ShaderEditorTab::init_type();
	TextBox::init_type();
	Slider::init_type();
	ListView::init_type();
	TileView::init_type();
	TreeView::init_type();
	ToolBar::init_type();
	ContextMenu::init_type();
	Dialog::init_type();
	ConfirmationDialog::init_type();
	FileDialog::init_type();
	ColorPickDialog::init_type();
	ConsoleTab::init_type();
	ExplorerTab::init_type();
	ContentTab::init_type();
	PropertyTab::init_type();
	GamePreviewTab::init_type();
	ToolTab::init_type();
	PropertyView::init_type();
	WorldView::init_type();
	MessageDialog::init_type();
	TypePickDialog::init_type();
	TextButton::init_type();
	ColorField::init_type();
	Texture2DField::init_type();
	ObjectField::init_type();
	EditableLabel::init_type();
	NumberField::init_type();
	TextField::init_type();
	Vec2Field::init_type();
	Vec3Field::init_type();
	Vec4Field::init_type();
	TransformField::init_type();
	PropertyControl::init_type();
	ComboBox::init_type();
	
	//Resources
	Font::init_type();
	Texture::init_type();
	Texture2D::init_type();
	Shader::init_type();
	Music::init_type();
	SoundEffect::init_type();
	TextFile::init_type();


	// Bind all methods.
	MethodMaster::init();

	Project::bind_methods();
	Scene::bind_methods();
	Layer::bind_methods();

	WorldObject::bind_methods();
	PointLight::bind_methods();
	DirectionalLight::bind_methods();
	ConeLight::bind_methods();
	Sprite::bind_methods();
	World::bind_methods();
	Camera::bind_methods();
	Terrain::bind_methods();
	TerrainBrush::bind_methods();
	Water::bind_methods();
	Sky::bind_methods();
	Clouds::bind_methods();
	Vegetation::bind_methods();
	Model::bind_methods();
	Mesh::bind_methods();
	Material::bind_methods();
	Environment::bind_methods();

	Scriptable::bind_methods();
	TitanScript::bind_methods();
	Mouse::bind_methods();
	Event::bind_methods();
	InputEvent::bind_methods();
	UIEvent::bind_methods();
	CollisionEvent::bind_methods();
	Keyboard::bind_methods();
	Key::bind_methods();
	Time::bind_methods();
	RigidBody2D::bind_methods();
	RigidBody3D::bind_methods();
	CollisionShape2D::bind_methods();
	CircleShape2D::bind_methods();
	BoxShape2D::bind_methods();
	TransformComponent::bind_methods();
	RenderComponent::bind_methods();
	Component::bind_methods();
	Viewport::bind_methods();
	EditorViewport::bind_methods();
	ContentManager::bind_methods();
	ListView::bind_methods();
	TileView::bind_methods();
	TreeView::bind_methods();
	EditorApp::bind_methods();
	PropertyTab::bind_methods();
	GamePreviewTab::bind_methods();
	ToolTab::bind_methods();
	PropertyView::bind_methods();
	WorldView::bind_methods();
	TextButton::bind_methods();
	ColorField::bind_methods();
	Texture2DField::bind_methods();
	ObjectField::bind_methods();
	EditableLabel::bind_methods();
	NumberField::bind_methods();
	TextField::bind_methods();
	Vec2Field::bind_methods();
	Vec3Field::bind_methods();
	Vec4Field::bind_methods();
	TransformField::bind_methods();
	PropertyControl::bind_methods();

	Node::bind_methods();
	Canvas::bind_methods();

	Label::bind_methods();
	Button::bind_methods();
	LabelButton::bind_methods();
	ImageButton::bind_methods();
	TextButton::bind_methods();
	Toggle::bind_methods();
	ToggleStrip::bind_methods();
	Tab::bind_methods();
	TextEditorTab::bind_methods();
	ShaderEditorTab::bind_methods();
	TextBox::bind_methods();
	Slider::bind_methods();
	ToolBar::bind_methods();
	ContextMenu::bind_methods();
	Dialog::bind_methods();
	ConfirmationDialog::bind_methods();
	FileDialog::bind_methods();
	ColorPickDialog::bind_methods();
	MessageDialog::bind_methods();
	TypePickDialog::bind_methods();
	ConsoleTab::bind_methods();
	ExplorerTab::bind_methods();
	ContentTab::bind_methods();
	ComboBox::bind_methods();

	//Resources
	Font::bind_methods();
	Texture::bind_methods();
	Texture2D::bind_methods();
	Shader::bind_methods();
	Music::bind_methods();
	SoundEffect::bind_methods();
	TextFile::bind_methods();

	MMASTER->add_inherited_methods();
}