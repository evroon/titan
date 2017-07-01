#pragma once

#include "resources/Resource.h"
#include "game/Scene.h"

class Project : public Resource
{
	OBJ_DEFINITION(Project, Resource);

public:
	Project();
	Project(Scene* p_main_scene);

	virtual ~Project();

	String serialize() const;
	void write();
	void load(const File& p_file);

private:
	Vector<Scene> scenes;
	Scene* default_scene;
};

