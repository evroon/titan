#include "UBO.h"

#include "resources/Texture.h"

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
	data_size = p_buffer.size();
	data = new float[data_size];
	
	for (int c = 0; c < p_buffer.size(); c++)
		data[c] = p_buffer[c];
	
	init();
}

void UBO::set_data(const Array<mat4>& p_buffer)
{
	GLuint ubo = 0;
	data_size = p_buffer.size() * 16;
	data = new float[data_size];

	for (int c = 0; c < p_buffer.size(); c++)
		for (int i = 0; i < 16; i++)
			data[c * 16 + i] = p_buffer[c].m[i];

	init();
}

void UBO::init()
{
	GLuint ubo = 0;
	glGenBuffers(1, &ubo);
	bind();
	glBufferData(GL_UNIFORM_BUFFER, data_size * sizeof(float), &data[0], GL_STATIC_DRAW);
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
