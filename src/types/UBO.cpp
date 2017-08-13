#include "UBO.h"



UBO::UBO()
{
	glBindBuffer(GL_UNIFORM_BUFFER, gbo);
	GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(p, &shader_data, sizeof(shader_data))
		glUnmapBuffer(GL_UNIFORM_BUFFER);

}


UBO::~UBO()
{
}
