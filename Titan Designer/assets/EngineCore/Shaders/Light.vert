#version 150

attribute vec4 Position;
varying vec2 pos;
uniform float radius;
uniform mat4 model;

void main()
{
	pos = Position.xy / 1;
	gl_Position = model * vec4(Position.xy * 2.0, Position.zw);
}