#version 330


layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texCoord;
layout (location = 2) in vec3 in_normal;

out vec2 texCoord;

uniform mat4 transformationMatrix;

layout (std140) uniform engineBlock {
	mat4 projectionMatrix;
	mat4 viewMatrix;

}nsBlock;

void main() {
	
	vec4 worldPosition = transformationMatrix * vec4(in_position, 1.0);
	gl_Position = nsBlock.projectionMatrix * nsBlock.viewMatrix * worldPosition;
	//gl_Position = vec4(in_position, 1.0);
	
	texCoord = in_texCoord;

}
