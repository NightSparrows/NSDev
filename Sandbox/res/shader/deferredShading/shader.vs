#version 440

layout (location = 0) in vec2 in_position;

out vec2 texCoord;

void main() {

	gl_Position = vec4(in_position, 0.0, 1.0);
	
	texCoord = in_position * 0.5 + 0.5;
	
}