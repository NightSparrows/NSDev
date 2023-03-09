#version 330

const int MAX_LIGHTS = 8;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texCoord;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec3 in_tangent;

out vec2 vs_texCoord;

const int MAX_INSTANCES = 10000;

uniform mat4 transformationMatrix[MAX_INSTANCES];

layout (std140) uniform engineUbo {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec3 fogColor;
	float fogDensity;
	float fogGradient;
	Light lights[MAX_LIGHTS];
}nsBlock;

void main() {

	vec4 worldPosition = transformationMatrix[gl_Instances] * vec4(in_position, 1.0);
	vec4 positionRelativeToCam = nsBlock.viewMatrix * worldPosition;
	gl_Position = nsBlock.projectionMatrix * positionRelativeToCam;
	
	vs_texCoord = in_texCoord;
	
}

