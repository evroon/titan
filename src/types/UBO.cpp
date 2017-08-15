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
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(p_buffer), &p_buffer, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBO::update_data(const Array<float>& p_buffer)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);

	GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(p, &p_buffer, sizeof(p_buffer));

	glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void UBO::bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
}

void UBO::bindBufferBase()
{
	glBindBufferBase(GL_UNIFORM_BUFFER, binding_point_index, ubo);
}

void UBO::bindBufferBase(int index)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, ubo);
}