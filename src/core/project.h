#pragma once

#include "resources/resource.h"
#include "resources/textfile.h"
#include "game/scene.h"
#include "graphics/renderer.h"

class Project : public Node
{
	OBJ_DEFINITION(Project, Node);

public:
	Project();
	Project(const String& p_file);

	virtual ~Project();

	void create();
	void load();
	void save();
	void save_as(const String& p_file);

	Scene* get_main_scene() const;

	String serialize() const;

	Renderer* get_renderer() const;
	void set_renderer(Renderer* p_renderer) const { }

	static void bind_methods();

private:
	Vector<Scene> scenes;
	Scene* default_scene;

	TextFile* text_file;
};