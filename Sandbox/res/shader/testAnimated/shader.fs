#version 330

const int MAX_LIGHTS = 8;

// in
in vec2 texCoord;
in vec3 normal;
in vec3 toLightVector[MAX_LIGHTS];
in vec3 toCameraVector;

// out
layout (location = 0) out vec4 out_Color;

// uniform
uniform sampler2D textureSampler;

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

const float gamma = 2.2;
const float exposure = 0.2;

void main() {

	vec4 texColor = texture(textureSampler, texCoord, -1.0);
	
	if (texColor.a < 0.1) {
		discard;
	}

	// pointing upward normal mapping handle
	vec3 unitNormal = vec3(0, 0, 1);
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
	
	out_Color = vec4(totalDiffuse, 1.0) * texColor;
	
	vec3 toneMapped = vec3(1.0) - exp(-out_Color.rgb * exposure);
	
	out_Color.rgb = pow(toneMapped, vec3(1.0f / gamma));
	
}
