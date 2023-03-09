#version 330

const int MAX_LIGHTS = 8;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texCoord;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec3 in_tangent;

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
const int MAX_INSTANCES = 600;
layout (std140) uniform instanceTransforms {
	mat4 matrices[MAX_INSTANCES];
}transform;

// shadow
uniform mat4 u_toShadowMapSpace;

const float shadowDistance = 150.0;		// recommend uniform it
const float transitionDistance = 10.0;

void main() {

	mat4 transformationMatrix = transform.matrices[gl_InstanceID];

	vec4 worldPosition = transformationMatrix * vec4(in_position, 1.0);
	
	mat4 modelViewMatrix = nsBlock.viewMatrix * transformationMatrix;
	//vec4 positionRelativeToCam = modelViewMatrix * vec4(in_position, 1.0);
	vec4 positionRelativeToCam = modelViewMatrix * vec4(in_position, 1.0);
	gl_Position = nsBlock.projectionMatrix * positionRelativeToCam;
	
	texCoord = in_texCoord;
	vec3 surfaceNormal = (modelViewMatrix * vec4(in_normal, 0.0)).xyz;
	
	vec3 norm = normalize(surfaceNormal);
	vec3 tang = normalize((modelViewMatrix * vec4(in_tangent, 0.0)).xyz);
	vec3 bitang = cross(norm, tang);
	
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

