#include "Project.h"

#include "core/Serializer.h"

#include "world/Terrain.h"
#include "graphics/Renderer.h"

Project::Project()
{
	//create();
}

Project::Project(const String& p_file)
{
	text_file = new TextFile(p_file);
	load();
}

Project::~Project()
{

}

void Project::create()
{
	default_scene = new Scene("main");
	scenes.push_back(default_scene);
	add_child(default_scene);
}

void Project::load()
{
	Serializer s;

	Variant project = s.deserialize(text_file->get_source());
	
	default_scene = project.operator Project*()->get_child_by_type<Scene*>();
	add_child(default_scene);
}

void Project::save()
{
	FBO2D* f = default_scene->get_child("Terrain")->cast_to_type<Terrain*>()->get_brush()->get_fbo();
	DEFERRED_RENDERER->save_fbo(f, "EngineCore/Textures", 0);

	Serializer s;
	String source = s.serialize(this);

	text_file->write(source);
}

void Project::save_as(const String& p_file)
{
	text_file = new TextFile(p_file);

	save();
}

Scene* Project::get_main_scene() const
{
	return default_scene;
}

String Project::serialize() const
{
	return String();
}

#undef CLASSNAME
#define CLASSNAME Project

void Project::bind_methods()
{
	REG_CSTR(0);
}