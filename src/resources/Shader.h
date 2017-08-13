#pragma once

#include <glew\glew.h>

#include "types\UBO.h"
#include "Resource.h"
#include "File.h"

class Shader : public Resource
{
	OBJ_DEFINITION(Shader, Resource)

public:
	Shader() = default;
    Shader(const String &filename);
    ~Shader();

    struct Uniform
	{
		GLfloat count;
		GLuint location;
		GLuint index;
		GLint size;
		GLsizei length;
		GLchar name[100];
		GLenum type;
	};

	struct Block
	{
		GLuint index;
		GLuint location;
		GLchar name[100];
		GLsizei length;
	};

	void load() override;
	void reload() override;
	void free() override;

    void start();
    void bind();
	void unbind();

	bool has_geometry_shader();

    void set_uniform(const String &name, const bool value);
    void set_uniform(const String &name, const int value);
    void set_uniform(const String &name, const float value);
	void set_uniform(const String &name, const double value);
    void set_uniform(const String &name, const vec2 &value);
	void set_uniform(const String &name, const vec2i &value);
    void set_uniform(const String &name, const vec3 &value);
	void set_uniform(const String &name, const vec3i &value);
    void set_uniform(const String &name, const vec4 &value);
	void set_uniform(const String &name, const vec4i &value);
	void set_uniform(const String &name, const Color &value);
    void set_uniform(const String &name, const mat4 &value);
	void set_uniform(const String & name, const Array<mat4>& value);

	void bind_buffer(const String& p_name, Ref<UBO> p_ubo);

	int get_program() const;

private:
	GLint create_shader(const String& p_path, GLenum ShaderType);
	void create_program();

    void set_info();

	int program_id;
	int vertexshader_id;
	int fragmentshader_id;
	int geometryshader_id;

	bool isvalid = false;

    Dictionary<String, Uniform> uniforms;
	Dictionary<String, Block> blocks;

	File vertex_path;
	File fragment_path;
	File geometry_path;
};