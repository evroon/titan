#include "Sky.h"

#include "Mesh.h"

#include "Core/ContentManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/View.h"

Sky::Sky()
{
	shader = CONTENT->LoadShader("EngineCore/Sky");

	set_size(vec3(100.0f, 100.0f, 100.0f));
	set_pos(vec3(0, 0, 0.0f));

	build_mesh();
}


Sky::~Sky()
{

}

void Sky::build_mesh()
{
	struct Vertex
	{
		vec3 position;
		vec3 normal;
		vec2 texcoords;
	};

	struct Face
	{
		unsigned indices[3];
	};

	std::vector<Vertex> vertices =
	{
		{ -1.0, -1.0,  1.0 },
		{ 1.0, -1.0,  1.0 },
		{ 1.0,  1.0,  1.0 },
		{ -1.0,  1.0,  1.0 },

		{ -1.0, -1.0, -1.0 },
		{ 1.0, -1.0, -1.0 },
		{ 1.0,  1.0, -1.0 },
		{ -1.0,  1.0, -1.0 }
	};

	std::vector<Face> indices =
	{
		{ 0, 1, 2 },
		{ 2, 3, 0 },

		{ 1, 5, 6 },
		{ 6, 2, 1 },

		{ 7, 6, 5 },
		{ 5, 4, 7 },

		{ 4, 0, 3 },
		{ 3, 7, 4 },

		{ 4, 5, 1 },
		{ 1, 0, 4 },

		{ 3, 2, 6 },
		{ 6, 7, 3 }
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Face), &indices, GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glBindVertexArray(0);
}

void Sky::draw()
{
	//glEnableVertexAttribArray(0);
	//glBindVertexArray(VAO);

	RENDERER->stop_culling();
	RENDERER->stop_depth_test();

	Transform t = get_transform();
	t.update();

	shader->Bind();
	shader->setUniform("view", RENDERER->RENDERER->get_final_matrix());
	shader->setUniform("light_position", vec3(0, 0, -1));
	shader->setUniform("ambient", Color::FromRGB(vec3i(66, 182, 244)).get_rgb());
	shader->setUniform("sun_color", vec3(1.0f));
	shader->setUniform("camera_position", ACTIVE_WORLD->get_active_camera()->get_pos());

	MeshHandler::get_singleton()->get_cube()->bind();
	MeshHandler::get_singleton()->get_cube()->draw();
	MeshHandler::get_singleton()->get_cube()->unbind();

	RENDERER->use_culling();
	RENDERER->use_depth_test(.5f, 1000.0f);

	//glBindVertexArray(0);
	//glDisableVertexAttribArray(0);
}
