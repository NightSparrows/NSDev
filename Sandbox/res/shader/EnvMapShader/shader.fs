#version 330

const int MAX_LIGHTS = 8;

in vec3 unitNormal;
in vec3 toLightVector[MAX_LIGHTS];
in vec3 toCameraVector;
in vec3 reflectedVector;
in vec3 refractedVector;
in float visibility;
in vec4 shadowCoords;

out vec4 out_Color;

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

uniform samplerCube u_envMapSampler;

// shadow
uniform sampler2D shadowMap;
// shadow improve PCF
const int pcfCount = 3;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
const float mapSize = 8192;

void main() {

	// Shadow
	// shadow map size
	float texelSize = 1.0 / mapSize;
	float total = 0.0;
	
	for(int x = -pcfCount; x <= pcfCount; x++) {
		for(int y = -pcfCount; y <= pcfCount; y++) {
			float objectNearestLight = texture(shadowMap, shadowCoords.xy + vec2(x, y) * texelSize).r;
			if (shadowCoords.z > objectNearestLight + 0.0015) {
				total += 1.0;
			}
		}
	}

	total /= totalTexels;

	float shadowFactor = 1.0 - (total * shadowCoords.w);
	// End Shadow settings

	out_Color = vec4(1.0);

	vec4 reflectedColor = texture(u_envMapSampler, reflectedVector);
	vec4 refractedColor = texture(u_envMapSampler, refractedVector);
	
	vec4 environColor = mix(reflectedColor, refractedColor, 0.5);

	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiffuse = vec3(0);

	for (int i = 0; i < nsBlock.lightCount; i++) {
		
		float distance = length(toLightVector[i]);
		vec3 attenuation = nsBlock.lights[i].attenuation.xyz;
		float attFactor = attenuation.x + (attenuation.y * distance) + (attenuation.z * distance * distance);
		
		vec3 unitLightVector = normalize(toLightVector[i]);
	
		float nDotl = dot(unitNormal, unitLightVector);
		float brightness = max(nDotl, 0.0);
		
		vec3 diffuse = (brightness * nsBlock.lights[i].color.xyz) / attFactor;
		
		totalDiffuse = totalDiffuse + diffuse;
	}
	totalDiffuse = max(totalDiffuse, 0.2);
	
	vec4 texColor = vec4(1.0, 1.0, 0.0, 1.0);
	
	out_Color = texColor * vec4(totalDiffuse, 1.0);
	//out_Color = mix(out_Color, reflectedColor, 0.6);
	out_Color = mix(out_Color, environColor, 0.9);


	out_Color = mix(vec4(nsBlock.fogColor.xyz, 1.0), out_Color, visibility);
}
