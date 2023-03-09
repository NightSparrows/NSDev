#version 330


layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texCoord;
layout (location = 2) in vec3 in_normal;

out vec2 texCoord;
out float visibility;
out vec3 fogColor;

out vec3 gPosition;
out vec3 gNormal;

const int MAX_LIGHTS = 8;

struct Light {
	vec3 position;
	vec3 color;
	vec3 attenuation;
};

layout (std140) uniform engineUbo {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec3 fogColor;
	float fogDensity;
	float fogGradient;
	Light lights[MAX_LIGHTS];
}nsBlock;

uniform mat4 transformationMatrix;

void main() {
	
	vec4 worldPosition = transformationMatrix * vec4(in_position, 1.0);
	vec4 positionRelativeToCam = nsBlock.viewMatrix * worldPosition;
	gl_Position = nsBlock.projectionMatrix * positionRelativeToCam;
	
	// gl_Position = vec4(in_position, 1.0);
	
	texCoord = in_texCoord;

	// fog Calc
	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance * nsBlock.fogDensity), nsBlock.fogGradient));
	visibility = clamp(visibility, 0.0, 1.0);

	fogColor = nsBlock.fogColor;
	
	gPosition = worldPosition.xyz;
	gNormal = normalize((transformationMatrix * vec4(in_normal, 0.0)).xyz);
	
}
