#version 330

const int MAX_LIGHTS = 8;

in vec3 in_position;
in vec2 in_texCoord;
in vec3 in_normal;
in vec3 in_tangent;

out vec3 unitNormal;
out vec3 toLightVector[MAX_LIGHTS];
out vec3 toCameraVector;
out vec3 reflectedVector;
out vec3 refractedVector;
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

uniform mat4 u_transformationMatrix;

uniform vec3 cameraPosition;

// shadow
uniform mat4 u_toShadowMapSpace;

const float shadowDistance = 150.0;		// recommend uniform it
const float transitionDistance = 10.0;

void main() {

	vec4 worldPosition = u_transformationMatrix * vec4(in_position, 1.0);
	vec4 positionRelativeToCam = nsBlock.viewMatrix * worldPosition;
	gl_Position = nsBlock.projectionMatrix * positionRelativeToCam;
	
	unitNormal = (u_transformationMatrix * vec4(in_normal, 0.0)).xyz;
	
	for(int i = 0; i < nsBlock.lightCount; i++) {
		toLightVector[i] = (nsBlock.lights[i].position.xyz - worldPosition.xyz);
	}
	
	toCameraVector = (-positionRelativeToCam.xyz);
	
	unitNormal = normalize(unitNormal);
	
	//vec3 camPosition = vec3(-nsBlock.viewMatrix[3][0], -nsBlock.viewMatrix[3][1], -nsBlock.viewMatrix[3][2]);
	
	vec3 viewVector = normalize(worldPosition.xyz - cameraPosition);
	reflectedVector = reflect(viewVector, unitNormal);
	refractedVector = refract(viewVector, unitNormal, 1.0 / 1.1);
	
	float distanceFromCam = length(positionRelativeToCam.xyz);
	
	visibility = exp(-pow((distanceFromCam * nsBlock.fogProperty.x), nsBlock.fogProperty.y));
	visibility = clamp(visibility, 0.0, 1.0);
	
	// shadow
	shadowCoords = u_toShadowMapSpace * worldPosition;
	float distance = distanceFromCam - (shadowDistance - transitionDistance);
	distance = distance / transitionDistance;
	shadowCoords.w = clamp(1.0 - distance, 0.0, 1.0);
	
}

