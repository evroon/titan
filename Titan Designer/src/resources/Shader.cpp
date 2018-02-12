#include "Shader.h"

#include <string>
#include <algorithm>
#include <fstream>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include "SDL.h"

#include "math/Math.h"
#include "math/Color.h"
#include "String.h"

#include "core/TMessage.h"

#include "core/ContentManager.h"

#define MAX_LOG_LENGTH 1000

Shader::Shader(const String &p_path)
{
	program_id = -1;
	vertexshader_id = -1;
	fragmentshader_id = -1;
	geometryshader_id = -1;
	tess_control_id = -1;
	tess_evaluation_id = -1;
	compute_id = -1;

	vertex_path = p_path + ".vert";
	fragment_path = p_path + ".frag";
	geometry_path = p_path + ".geom";
	tess_control_path = p_path + ".tc";
	tess_evaluation_path = p_path + ".te";
	compute_path = p_path + ".comp";

	file = p_path;

	load();
}

Shader::~Shader()
{
	free();
}

void Shader::load()
{
	isvalid = true;

	// compute shader
	if (is_compute_shader())
	{
		compute_id = create_shader(compute_path, GL_COMPUTE_SHADER);
		if (compute_id)
			create_program();
		return;
	}

	// vertex shader
	vertexshader_id = create_shader(vertex_path, GL_VERTEX_SHADER);
	if (!vertexshader_id) isvalid = false;

	// fragment shader
	fragmentshader_id = create_shader(fragment_path, GL_FRAGMENT_SHADER);
	if (!fragmentshader_id) isvalid = false;

	// geometry shader
	if (has_geometry_shader())
	{
		geometryshader_id = create_shader(geometry_path, GL_GEOMETRY_SHADER);
		if (!geometryshader_id) isvalid = false;
	}

	// tesselation shaders
	if (has_tesselation_shader())
	{
		tess_control_id = create_shader(tess_control_path, GL_TESS_CONTROL_SHADER);
		tess_evaluation_id = create_shader(tess_evaluation_path, GL_TESS_EVALUATION_SHADER);
		if (!tess_control_id || !tess_evaluation_id) isvalid = false;
	}

	if (isvalid)
		create_program();
	else
		T_ERROR("Failed to compile: " + get_file() + "!");
}

void Shader::reload()
{
	free();
	load();
}

void Shader::free()
{
	CONTENT->free_textfile(vertex_path);
	CONTENT->free_textfile(fragment_path);
	CONTENT->free_textfile(geometry_path);
	CONTENT->free_textfile(tess_control_path);
	CONTENT->free_textfile(tess_evaluation_path);
	CONTENT->free_textfile(compute_path);

	if (!isvalid)
		return;

	glDeleteShader(vertexshader_id);
	glDeleteShader(fragmentshader_id);

	if (geometryshader_id != -1)
		glDeleteShader(geometryshader_id);

	if (tess_control_id != -1 && tess_evaluation_id != -1)
	{
		glDeleteShader(tess_control_id);
		glDeleteShader(tess_evaluation_id);
	}

	if (compute_id != -1)
		glDeleteShader(compute_id);

	glDeleteProgram(program_id);
}

void Shader::start()
{
    glUseProgram(program_id);
}

GLint Shader::create_shader(const String& p_path, GLenum ShaderType)
{
    GLint shader_id = glCreateShader(ShaderType);
    GLchar infolog[MAX_LOG_LENGTH];
    const GLchar* shadersource;
    GLint infologlength;
    GLint compilestatus;

	TextFile* file = CONTENT->LoadTextFile(p_path);

	String s = file->get_source();
	shadersource = s.c_str();

    glShaderSource(shader_id, 1, &shadersource, NULL);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compilestatus);

    if (!compilestatus)
    {
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &infologlength);
        glGetShaderInfoLog(shader_id, MAX_LOG_LENGTH, NULL, infolog);

		String err = "Failed to compile: " + p_path;
		err += "\n\nStart InfoLog:\n\n";
		err += infolog;
		err += "End InfoLog\n";

		T_ERROR(err);

		shader_id = -1;
    }
    return shader_id;
}

void Shader::create_program()
{
    GLint linkStatus, infologlength;
    GLchar *infolog;
    program_id = glCreateProgram();

	if (compute_id != -1)
		glAttachShader(program_id, compute_id);
	else
	{
		glAttachShader(program_id, vertexshader_id);
		glAttachShader(program_id, fragmentshader_id);

		if (geometryshader_id != -1)
			glAttachShader(program_id, geometryshader_id);

		if (tess_control_id != -1 && tess_evaluation_id != -1)
		{
			glAttachShader(program_id, tess_control_id);
			glAttachShader(program_id, tess_evaluation_id);
		}
	}   

	glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &linkStatus);

    if (linkStatus != GL_TRUE)
    {
        T_ERROR("Program link failed for shader: " + file);

        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &infologlength);
        infolog = new GLchar(infologlength + 1);
        glGetProgramInfoLog(program_id, infologlength + 1, NULL, infolog);

        SDL_Log(infolog);
        delete infolog;
		return;
    } 
	else
    	 glUseProgram(program_id);

    set_info();
}

void Shader::bind()
{
	if (isvalid)
		glUseProgram(program_id);
}
void Shader::unbind()
{
	glUseProgram(0);
}

bool Shader::has_geometry_shader()
{
	return geometry_path.is_file();
}

bool Shader::has_tesselation_shader()
{
	return tess_control_path.is_file() && tess_evaluation_path.is_file();
}

bool Shader::is_compute_shader()
{
	return compute_path.is_file();
}

void Shader::set_info()
{
	Uniform uni;
    GLint count = 0;

    glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &count);

    for (GLint c = 0; c < count; c++)
    {
        glGetActiveUniform(program_id, c, sizeof(uni.name) - 1, &uni.length, &uni.size, &uni.type, uni.name);
        uni.location = glGetUniformLocation(program_id, uni.name);
        uni.index = c;
        uniforms[uni.name] = uni;
    }

	Block block;

	glGetProgramiv(program_id, GL_ACTIVE_UNIFORM_BLOCKS, &count);

	for (GLint c = 0; c < count; c++)
	{
		glGetActiveUniformBlockName(program_id, c, sizeof(block.name) - 1, &block.length, block.name);
		block.location = glGetUniformBlockIndex(program_id, block.name);
		block.index = c;
		blocks[block.name] = block;
	}
}

#if 0
#define CHECK_NAME \
if (uniforms[name].count == 0)\
	return;
#endif

#define CHECK_NAME \
if (!uniforms.contains(name))\
{\
	T_WARNING("uniform with name: " + name + " does not exist in shader: " + get_file());\
	return;\
}

void Shader::set_uniform(const String &name, bool value)
{
	CHECK_NAME
	
    glUniform1i(uniforms[name].location, value ? 1 : 0);
}

void Shader::set_uniform(const String &name, const int value)
{
	CHECK_NAME

    glUniform1i(uniforms[name].location, value);
}

void Shader::set_uniform(const String &name, const float value)
{
	CHECK_NAME

	glUniform1f(uniforms[name].location, value);
}

void Shader::set_uniform(const String &name, const double value) //BETA
{
	CHECK_NAME

	glUniform1f(uniforms[name].location, (float)value);
}

void Shader::set_uniform(const String &name, const vec2 &value)
{
	CHECK_NAME

    glUniform2f(uniforms[name].location, value.x, value.y);
}

void Shader::set_uniform(const String &name, const vec2i &value)
{
	CHECK_NAME

	glUniform2i(uniforms[name].location, value.x, value.y);
}

void Shader::set_uniform(const String &name, const vec3 &value)
{
	CHECK_NAME

    glUniform3f(uniforms[name].location, value.x, value.y, value.z);
}

void Shader::set_uniform(const String &name, const vec3i &value)
{
	CHECK_NAME

	glUniform3i(uniforms[name].location, value.x, value.y, value.z);
}

void Shader::set_uniform(const String &name, const vec4 &value)
{
	CHECK_NAME

    glUniform4f(uniforms[name].location, value.x, value.y, value.z, value.w);
}

void Shader::set_uniform(const String &name, const vec4i &value)
{
	CHECK_NAME

	glUniform4i(uniforms[name].location, value.x, value.y, value.z, value.w);
}
void Shader::set_uniform(const String &name, const Color &value)
{
	CHECK_NAME

	glUniform4f(uniforms[name].location, value.x, value.y, value.z, value.w);
}

void Shader::set_uniform(const String &name, const mat4 &value)
{
	CHECK_NAME

    glUniformMatrix4fv(uniforms[name].location, 1, false, &value.m[0]);
}

void Shader::set_uniform(const String &name, const Array<vec4> &value)
{
	CHECK_NAME

	glUniform4fv(uniforms[name].location, value.size(), &(&value[0])->x);
}

void Shader::set_uniform(const String &name, const Array<mat4> &value)
{
	CHECK_NAME

	glUniformMatrix4fv(uniforms[name].location, value.size(), false, &(&value[0])->m[0]);
}

void Shader::bind_block(const String& p_var_name, UBO* p_ubo)
{
	glUniformBlockBinding(program_id, blocks[p_var_name].location, p_ubo->get_bound_index());
}

int Shader::get_program() const
{
	return program_id;
}
