#include "Mesh.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "core/ContentManager.h"

#include "graphics/Renderer.h"

MeshHandler* MeshHandler::singleton = new MeshHandler;

//=========================================================================
//SimpleMesh
//=========================================================================

void SimpleMesh::init(const Array<Vertex>& p_vertices, const Array<Face>& p_faces, int p_type)
{
	draw_type = p_type;
	vertices_count = p_vertices.size();
	faces_count = p_faces.size();

	vertices = new Vertex[vertices_count];
	faces = new Face[faces_count];

	for (int c = 0; c < p_vertices.size(); c++)
		vertices[c] = p_vertices[c];

	for (int c = 0; c < p_faces.size(); c++)
		faces[c] = p_faces[c];

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * faces_count, &faces[0], GL_STATIC_DRAW);
}

SimpleMesh::~SimpleMesh()
{
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
}

void SimpleMesh::set_plane()
{
	const Array<Vertex> vertices =
	{
		{ 1, -1, 0 },
		{ 1, 1, 0 },
		{ -1, 1, 0 },
		{ -1, -1, 0 }
	};

	const Array<Face> faces =
	{
		{ 0, 1, 2 },
		{ 2, 3, 0 }
	};

	init(vertices, faces, GL_TRIANGLES);
}

void SimpleMesh::set_line()
{
	const Array<Vertex> vertices =
	{
		{ 0, 0, 0 },
		{ 1, 0, 0 }
	};

	const Array<Face> faces =
	{
		{ 0, 1 }
	};

	init(vertices, faces, GL_LINES);
}

void SimpleMesh::set_cube()
{
	Array<Vertex> vertices;
	Array<Face> faces;

	vertices.reserve(8);
	faces.reserve(12);

	vertices.push_back({ -1.0, -1.0,  1.0 });
	vertices.push_back({ 1.0, -1.0, 1.0 });
	vertices.push_back({ 1.0, 1.0, 1.0 });
	vertices.push_back({ -1.0, 1.0, 1.0 });

	vertices.push_back({ -1.0, -1.0, -1.0 });
	vertices.push_back({ 1.0, -1.0, -1.0 });
	vertices.push_back({ 1.0, 1.0, -1.0 });
	vertices.push_back({ -1.0, 1.0, -1.0 });

	faces.push_back({ 1, 5, 6 });
	faces.push_back({ 6, 2, 1 });

	faces.push_back({ 0, 1, 2 });
	faces.push_back({ 2, 3, 0 });

	faces.push_back({ 7, 6, 5 });
	faces.push_back({ 5, 4, 7 });

	faces.push_back({ 4, 0, 3 });
	faces.push_back({ 3, 7, 4 });

	faces.push_back({ 4, 5, 1 });
	faces.push_back({ 1, 0, 4 });

	faces.push_back({ 3, 2, 6 });
	faces.push_back({ 6, 7, 3 });

	init(vertices, faces, GL_TRIANGLES);
}

void SimpleMesh::SetAttributes(Shader *shader)
{
	position_id = glGetAttribLocation(shader->get_program(), "a_position");

	if (position_id < 0)
		T_ERROR("attribute location invalid");

	glEnableVertexAttribArray(position_id);
	glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void SimpleMesh::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void SimpleMesh::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SimpleMesh::draw()
{
	glEnableVertexAttribArray(0);

	glDrawElements(draw_type, faces_count * (draw_type == GL_TRIANGLES ? 3 : 2), GL_UNSIGNED_BYTE, 0);

	glDisableVertexAttribArray(0);
}

void SimpleMesh::draw_instanced(int p_count)
{
	glEnableVertexAttribArray(0);

	glDrawElementsInstanced(draw_type, faces_count * (draw_type == GL_TRIANGLES ? 3 : 2), GL_UNSIGNED_BYTE, 0, p_count);

	glDisableVertexAttribArray(0);
}

//=========================================================================
//Mesh
//=========================================================================

Mesh::Mesh()
{

}

Mesh::Mesh(const String & p_path)
{
	import(p_path);
}

Mesh::~Mesh()
{
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
}

bool Mesh::import(const String& p_filepath)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(p_filepath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals);

	if (!scene)
	{
		T_ERROR(importer.GetErrorString());
		return false;
	}

	for (unsigned c = 0; c < scene->mNumMeshes; c++)
	{
		MeshNode* node = new MeshNode;
		node->init(scene->mMeshes[c]);
		node->parent = this;
		meshes.push_back(node);
	}

	for (unsigned c = 0; c < scene->mNumTextures; c++)
		textures.push_back(new Texture2D(scene->mTextures[c]));

	for (unsigned c = 0; c < scene->mNumMaterials; c++)
	{
		Material* material = new Material;
		material->load_material(scene->mMaterials[c]);
		materials.push_back(material);
	}

	for (int c = 0; c < meshes.size(); c++)
	{
		unsigned mat_index = meshes[c]->mat_index;
		meshes[c]->material = materials[mat_index];
	}

	return true;
}

void Mesh::draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (MeshNode* mesh : meshes)
		mesh->draw();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

//=========================================================================
//MeshNode
//=========================================================================

void Mesh::MeshNode::draw()
{
	glBindVertexArray(VAO);

	if (material)
	{
		material->shader->Bind();

		if (material->diffuse_texture)
			material->diffuse_texture->Bind(0);

		material->shader->set_uniform("color", material->diffuse_color);
	}

	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Mesh::MeshNode::init(aiMesh * p_mesh)
{
	mat_index = p_mesh->mMaterialIndex;

	//add vertices
	for (GLuint i = 0; i < p_mesh->mNumVertices; i++)
	{
		Vertex vertex;

		aiVector3D v = p_mesh->mVertices[i];
		aiVector3D n = p_mesh->mNormals[i];
		aiVector3D t;

		if (p_mesh->mTextureCoords[0])
			t = p_mesh->mTextureCoords[0][i];

		vertex.position = vec3(v.x, v.y, v.z);
		vertex.normal = vec3(n.x, n.y, n.z);
		vertex.texcoords = vec2(t.x, 1.0f - t.y);

		vertices.push_back(vertex);
	}

	//add faces
	for (unsigned i = 0; i < p_mesh->mNumFaces; i++)
	{
		Face face;

		for (int c = 0; c < 3; c++)
			face.indices[c] = p_mesh->mFaces[i].mIndices[c];

		faces.push_back(face);
	}

	setup_buffers();
}

void Mesh::MeshNode::setup_buffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(Face), &faces[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));

	glBindVertexArray(0);
}

//=========================================================================
//Material
//=========================================================================

void Material::load_material(const aiMaterial* p_material)
{
	//initialize values
	aiString name;
	aiColor3D color;
	float value;

	p_material->Get(AI_MATKEY_NAME, name);
	name = name.C_Str();

	p_material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	diffuse_color = Color(color.r, color.g, color.b);

	p_material->Get(AI_MATKEY_COLOR_SPECULAR, color);
	specular_color = Color(color.r, color.g, color.b);

	p_material->Get(AI_MATKEY_COLOR_AMBIENT, color);
	ambient_color = Color(color.r, color.g, color.b);

	p_material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
	emissive_color = Color(color.r, color.g, color.b);

	p_material->Get(AI_MATKEY_SHININESS, value);
	shininess = value;

	p_material->Get(AI_MATKEY_OPACITY, value);
	opacity = value;

	//initialize textures
	diffuse_texture = load_texture(p_material, aiTextureType_DIFFUSE);
	specular_texture = load_texture(p_material, aiTextureType_SPECULAR);
	ambient_texture = load_texture(p_material, aiTextureType_AMBIENT);

	diffuse_texture = CONTENT->LoadTexture("Models/672a8302d72f34f79980861feb2bdcd5/Arch73_06_Diff.jpg");

	shader = CONTENT->LoadShader("EngineCore/Shader3D");
}

Texture2D* Material::load_texture(const aiMaterial* p_material, const aiTextureType &p_type)
{
	if (p_material->GetTextureCount(p_type) > 0)
	{
		aiString Path;

		if (p_material->GetTexture(p_type, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			return CONTENT->LoadTexture(String(Path.C_Str()));
	}
	return NULL;
}

//=========================================================================
//MeshHandler
//=========================================================================

void MeshHandler::Init()
{
	plane = new SimpleMesh;
	plane->set_plane();

	line = new SimpleMesh;
	line->set_line();

	cube = new SimpleMesh;
	cube->set_cube();

	//plane->bind();
}

SimpleMesh* MeshHandler::get_plane() const
{
	return plane;
}

SimpleMesh* MeshHandler::get_line() const
{
	return line;
}

SimpleMesh* MeshHandler::get_cube() const
{
	return cube;
}

MeshHandler* MeshHandler::get_singleton()
{
	return singleton;
}