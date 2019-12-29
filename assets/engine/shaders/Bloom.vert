#version 330

attribute vec4 a_position;

out vec2 texcoords;

void main(){
	texcoords = a_position.xy * vec2(0.5) + vec2(0.5);
	gl_Position = vec4(a_position.xy, 0.0, 1.0);
}