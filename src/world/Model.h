#pragma once

#include "WorldObject.h"
#include "Mesh.h"

class Model : public WorldObject
{
	OBJ_DEFINITION(Model, WorldObject);

public:
	Model();
	Model(const String& p_path);
	Model(Mesh* p_mesh);
	
	virtual ~Model();

	void draw() override;
	void shadow_draw() override;

	void load_mesh(const String& p_path);

	void set_mesh(Mesh* p_mesh);
	Mesh* get_mesh() const;

	void set_shader(Shader* p_shader);
	Shader* get_shader() const;

private:
	Mesh* mesh;
	Shader* shader;
};

