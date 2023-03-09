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

// normal mapping
uniform float hasNormalMap;
uniform sampler2D normalSampler;

// extra info map (has specular)
uniform float hasExtraInfoMap;
uniform sampler2D u_extraInfoMap;

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

// I think it is an attribute of camera
const float gamma = 2.2;
const float exposure = 1.0;

//shadow 
uniform sampler2D shadowMap;
// shadow improve PCF
const int pcfCount = 2;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
const float mapSize = 4096;

void main() {

	// Shadow
	// shadow map size
	float texelSize = 1.0 / mapSize;
	float total = 0.0;
	
	for(int x = -pcfCount; x <= pcfCount; x++) {
		for(int y = -pcfCount; y <= pcfCount; y++) {
			float objectNearestLight = texture(shadowMap, shadowCoords.xy + vec2(x, y) * texelSize).r;
			if (shadowCoords.z > objectNearestLight + 0.0021) {
				total += 1.0;
			}
		}
	}

	total /= totalTexels;

	float shadowFactor = 1.0 - (total * shadowCoords.w);
	// End Shadow settings

	vec4 texColor = texture(textureSampler, texCoord);
	
	if (texColor.a < 0.1) {
		discard;
	}

	// pointing upward normal mapping handle
	vec3 unitNormal;
	if (hasNormalMap > 0.5) {
		unitNormal = 2.0 * texture(normalSampler, texCoord).rgb - 1.0;
		unitNormal = normalize(unitNormal);
	} else {
		unitNormal = vec3(0, 0, 1);
	}
	
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiffuse = vec3(0);
	vec3 totalSpecular = vec3(0);

	for (int i = 0; i < nsBlock.lightCount; i++) {
		vec3 lightColor = nsBlock.lights[i].color.xyz;
		
		float distance = length(toLightVector[i]);
		vec3 attenuation = nsBlock.lights[i].attenuation.xyz;
		float attFactor = attenuation.x + (attenuation.y * distance) + (attenuation.z * distance * distance);
		
		vec3 unitLightVector = normalize(toLightVector[i]);
	
		float nDotl = dot(unitNormal, unitLightVector);
		float brightness = max(nDotl, 0.0);
		
		vec3 diffuse = (brightness * lightColor) / attFactor;
		
		totalDiffuse = totalDiffuse + diffuse;
		
		// specular
		vec3 lightDirection = -unitLightVector;
		vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
		float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
		specularFactor = max(specularFactor, 0.0);
		float dampedFactor = pow(specularFactor, 10/* shineDamper */);
		totalSpecular = totalSpecular + ((dampedFactor * 0.5/* reflectivity */ * lightColor) / attFactor);
		
	}
	totalDiffuse = max(totalDiffuse * shadowFactor, 0.2);
	totalSpecular = max(totalSpecular * shadowFactor, 0.0);
	
	// extra info map
	if (hasExtraInfoMap > 0.5) {
		vec4 mapInfo = texture(u_extraInfoMap, texCoord);
		totalSpecular *= mapInfo.r;
	}
	
	out_Color = vec4(totalDiffuse, 1.0) * texColor + vec4(totalSpecular, 1.0);
	
	//out_Color = texColor;
	
	// Need to be write in post processing
	//vec3 toneMapped = vec3(1.0) - exp(-out_Color.rgb * exposure);
	
	//out_Color.rgb = pow(toneMapped, vec3(1.0f / gamma));
	
	// final
	// fogColor
	out_Color = mix(vec4(nsBlock.fogColor.xyz, 1.0), out_Color, visibility);
	
	
}
