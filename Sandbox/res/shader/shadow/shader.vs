#version 330

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texCoord;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec3 in_tangent;
layout (location = 4) in ivec4 in_bone;
layout (location = 5) in vec4 in_weight;

uniform mat4 u_transformationMatrix;
uniform mat4 u_projectionViewMatrix;
uniform ivec2 u_boneOffsets;
uniform float u_isStatic;

struct Light {
	vec4 position;
	vec4 color;
	vec4 attenuation;
};

const int MAX_LIGHTS = 8;

// Master Renderer uniform
layout (std140) uniform engineUbo {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 fogColor;
	vec2 fogProperty;	// density, gradient
	float lightCount;
	float keep;
	Light lights[MAX_LIGHTS];
}nsBlock;

// pose matrices
const int MAX_BONES = 600;
const int MAX_WEIGHTS = 4;
layout (std140) uniform boneTransforms {
	mat4 matrices[MAX_BONES];
}bone;

void main() {

	vec4 totalLocalPos;
	
	if (u_isStatic > 0.5) {
		totalLocalPos = vec4(in_position, 1.0);
	} else {	
		mat4 boneTransform = bone.matrices[in_bone[0] - u_boneOffsets.x] * in_weight[0];
		
		for(int i = 1; i < MAX_WEIGHTS; i++) {
			boneTransform += bone.matrices[in_bone[i] - u_boneOffsets.x] * in_weight[i];
		}
		
		totalLocalPos = boneTransform * vec4(in_position, 1.0);
	}
	gl_Position = u_projectionViewMatrix * u_transformationMatrix * totalLocalPos;


}


