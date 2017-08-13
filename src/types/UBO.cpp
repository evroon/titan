#include "UBO.h"

#include "resources\Texture.h"

UBO::UBO()
{
	glGenBuffers(1, &ubo);
}

UBO::~UBO()
{

}

void UBO::set_data(const Array<float>& p_buffer)
{
	GLuint ubo = 0;
	glGenBuffers(1, &ubo);
	bind();
	glBufferData(GL_UNIFORM_BUFFER, p_buffer.size() * sizeof(float), &p_buffer, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBO::set_data(const Array<mat4>& p_buffer)
{
	GLuint ubo = 0;
	glGenBuffers(1, &ubo);
	bind();
	glBufferData(GL_UNIFORM_BUFFER, p_buffer.size() * 16 * sizeof(float), &p_buffer, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBO::update_data(const Array<float>& p_buffer)
{
	bind();

	GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(p, &p_buffer, p_buffer.size() * sizeof(float));

	glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void UBO::update_data(const Array<mat4>& p_buffer)
{
	bind();

	GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(p, &p_buffer, p_buffer.size() * 16 * sizeof(float));

	glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void UBO::bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
}

void UBO::bind_at_index(int p_index)
{
	bound_index = p_index;
	glBindBufferBase(GL_UNIFORM_BUFFER, bound_index, ubo);
}

int UBO::get_bound_index() const
{
	return bound_index;
}
