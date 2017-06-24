#include "Project.h"



Project::Project()
{
}

Project::Project(Scene * p_main_scene)
{
	default_scene = p_main_scene;
	scenes.push_back(p_main_scene);

}


Project::~Project()
{
}

String Project::serialize() const
{
	return String();
}

void Project::write()
{
}

void Project::load(const File & p_file)
{
}
