#version 330

const int MAX_LIGHTS = 8;

layout (location = 0) in vec3 in_position;

// out
out vec3 texCoord;

struct Light {
	vec4 position;
	vec4 color;
	vec4 attenuation;
};

// Master Renderer uniform
layout (std140) uniform engineUbo {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 fogColor;
	vec2 fogProperty;
	float lightCount;
	float keep;
	Light lights[MAX_LIGHTS];
}nsBlock;

// transformation matrix
uniform mat4 u_transformationMatrix;

void main() {

	mat4 fixViewMatrix = mat4(nsBlock.viewMatrix);
	fixViewMatrix[3][0] = 0;
	fixViewMatrix[3][1] = 0;
	fixViewMatrix[3][2] = 0;
	
	vec4 pos = nsBlock.projectionMatrix * fixViewMatrix * vec4(in_position, 1.0);
	gl_Position = pos;
	
	texCoord = vec3(in_position.x, in_position.y, -in_position.z);

}

