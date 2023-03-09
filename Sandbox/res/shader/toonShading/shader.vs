#version 330

const int MAX_LIGHTS = 8;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texCoord;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec3 in_tangent;
layout (location = 4) in ivec4 in_bone;
layout (location = 5) in vec4 in_weight;

// out
out vec2 texCoord;
out vec3 normal;
out vec3 toLightVector[MAX_LIGHTS];
out vec3 toCameraVector;
out float visibility;
out vec4 shadowCoords;

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
uniform ivec2 u_boneOffsets;

// pose matrices
const int MAX_BONES = 600;
const int MAX_WEIGHTS = 4;
layout (std140) uniform boneTransforms {
	mat4 matrices[MAX_BONES];
}bone;

// shadow
uniform mat4 u_toShadowMapSpace;

const float shadowDistance = 150.0;		// recommend uniform it
const float transitionDistance = 10.0;

void main() {

	mat4 boneTransform = bone.matrices[in_bone[0] - u_boneOffsets.x] * in_weight[0];
	
	for(int i = 1; i < MAX_WEIGHTS; i++) {
		boneTransform += bone.matrices[in_bone[i] - u_boneOffsets.x] * in_weight[i];
	}
	
	vec4 totalLocalPos;
	vec4 totalNormalVec;
	totalLocalPos = boneTransform * vec4(in_position, 1.0);
	totalNormalVec = boneTransform * vec4(in_normal, 0.0);
	
	vec4 worldPosition = u_transformationMatrix * totalLocalPos;
	mat4 modelViewMatrix = nsBlock.viewMatrix * u_transformationMatrix;
	//vec4 positionRelativeToCam = modelViewMatrix * vec4(in_position, 1.0);
	vec4 positionRelativeToCam = modelViewMatrix * totalLocalPos;
	gl_Position = nsBlock.projectionMatrix * positionRelativeToCam;
	
	texCoord = in_texCoord;
	vec3 surfaceNormal = (modelViewMatrix * totalNormalVec).xyz;
	
	vec3 norm = normalize(surfaceNormal);
	vec3 tang = normalize((modelViewMatrix * vec4(in_tangent, 0.0)).xyz);
	vec3 bitang = normalize(cross(norm, tang));
	
	mat3 toTangentSpace = mat3(
		tang.x, bitang.x, norm.x,
		tang.y, bitang.y, norm.y,
		tang.z, bitang.z, norm.z
	);
	
	
	for(int i = 0; i < nsBlock.lightCount; i++) {
		vec3 lightPositionEyeSpace = (nsBlock.viewMatrix * vec4(nsBlock.lights[i].position.xyz, 1.0)).xyz;
		toLightVector[i] = toTangentSpace * (lightPositionEyeSpace - positionRelativeToCam.xyz);
	}
	
	toCameraVector = toTangentSpace * (-positionRelativeToCam.xyz);
	
	float distanceFromCam = length(positionRelativeToCam.xyz);
	
	visibility = exp(-pow((distanceFromCam * nsBlock.fogProperty.x), nsBlock.fogProperty.y));
	visibility = clamp(visibility, 0.0, 1.0);
	
	// shadow
	shadowCoords = u_toShadowMapSpace * worldPosition;
	float distance = distanceFromCam - (shadowDistance - transitionDistance);
	distance = distance / transitionDistance;
	shadowCoords.w = clamp(1.0 - distance, 0.0, 1.0);
	
}

