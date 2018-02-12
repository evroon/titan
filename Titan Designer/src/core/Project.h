#pragma once

#include "resources/Resource.h"
#include "resources/TextFile.h"
#include "game/Scene.h"

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

	static void bind_methods();
	
private:
	Vector<Scene> scenes;
	Scene* default_scene;

	TextFile* text_file;
};