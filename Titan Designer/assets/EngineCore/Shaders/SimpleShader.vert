attribute vec4 Position;
uniform mat4 model;

void main()
{
	gl_Position = model * Position;
}