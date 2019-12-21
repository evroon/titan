attribute vec4 Position;
varying vec2 texcoords;
uniform mat4 model;

void main(){
	texcoords = Position.xy * vec2(0.5) + vec2(0.5);
	gl_Position = model * Position;
}