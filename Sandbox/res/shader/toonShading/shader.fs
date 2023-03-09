#version 330

const int MAX_LIGHTS = 8;

// in
in vec2 texCoord;
in vec3 normal;
in vec3 toLightVector[MAX_LIGHTS];
in vec3 toCameraVector;
in float visibility;
in vec4 shadowCoords;

// out
layout (location = 0) out vec4 out_Color;

// uniform
uniform sampler2D textureSampler;

// diffuse
uniform float u_hasDiffuseTexture;
uniform vec4 u_diffuseColor;

// shadow
uniform sampler2D shadowMap;
// shadow improve PCF
const int pcfCount = 3;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
const float mapSize = 8192;


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
const float exposure = 0.5;

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
	
	// diffuse setting
	vec4 texColor;

	if (u_hasDiffuseTexture > 0.5) {
		texColor = texture(textureSampler, texCoord);
	} else {
		texColor = u_diffuseColor;
	}
	
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
	
	//out_Color = vec4(totalDiffuse, 1.0) * texColor;
	
	shadowFactor = max(shadowFactor, 0.7);
	
	out_Color = texColor * shadowFactor - (0.3 + (0.7 - shadowFactor)) * (vec4(1.0) - vec4(0.99216, 0.898, 0.898, 1.0));
	
	//vec3 toneMapped = vec3(1.0) - exp(-out_Color.rgb * exposure);
	
	//out_Color.rgb = pow(toneMapped, vec3(1.0f / gamma));
	out_Color = mix(vec4(nsBlock.fogColor.xyz, 1.0), out_Color, visibility);
	
}
