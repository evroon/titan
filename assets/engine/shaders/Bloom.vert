#version 330

attribute vec4 Position;

uniform mat4 model;

out vec2 texcoords;

void main(){
	texcoords = Position.xy * vec2(0.5) + vec2(0.5);
	gl_Position = model * Position;
}