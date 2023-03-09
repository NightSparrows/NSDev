#version 330

const int MAX_LIGHTS = 8;

// in
in vec2 texCoord;
in vec3 surfaceNormal;
in vec3 toLightVector[MAX_LIGHTS];
in vec3 toCameraVector;
in float visibility;
in vec4 shadowCoords;

// out
layout (location = 0) out vec4 out_Color;

// uniform
uniform sampler2D blendMap;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D bgTexture;

//shadow 
uniform sampler2D shadowMap;

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

// shadow improve PCF
const int pcfCount = 2;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
const float mapSize = 4096;

void main() {

	// shadow map size
	float texelSize = 1.0 / mapSize;
	float total = 0.0;
	
	for(int x = -pcfCount; x <= pcfCount; x++) {
		for(int y = -pcfCount; y <= pcfCount; y++) {
			float objectNearestLight = texture(shadowMap, shadowCoords.xy + vec2(x, y) * texelSize).r;
			if (shadowCoords.z > objectNearestLight) {
				total += 1.0;
			}
		}
	}

	total /= totalTexels;

	float shadowFactor = 1.0 - (total * shadowCoords.w);


	vec4 blendMapColor = texture(blendMap, texCoord);
	
	float bgTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = texCoord * 40.0;
	vec4 bgTextureColor = texture(bgTexture, tiledCoords) * bgTextureAmount;
	vec4 rTextureColor = texture(rTexture, tiledCoords) * blendMapColor.r;
	vec4 gTextureColor = texture(gTexture, tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(bTexture, tiledCoords) * blendMapColor.b;

	vec4 texColor = bgTextureColor + rTextureColor + gTextureColor + bTextureColor;

	// pointing upward normal mapping handle
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiffuse = vec3(0);
	vec3 totalSpecular = vec3(0);

	for (int i = 0; i < nsBlock.lightCount; i++) {
		
		float distance = length(toLightVector[i]);
		vec3 attenuation = nsBlock.lights[i].attenuation.xyz;
		float attFactor = attenuation.x + (attenuation.y * distance) + (attenuation.z * distance * distance);
		
		vec3 unitLightVector = normalize(toLightVector[i]);
	
		float nDotl = dot(unitNormal, unitLightVector);
		float brightness = max(nDotl, 0.0);
		
		vec3 diffuse = (brightness * nsBlock.lights[i].color.xyz) / attFactor;
		totalDiffuse = totalDiffuse + diffuse;
		
		// specular
		vec3 lightDirection = -unitLightVector;
		vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
		float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
		specularFactor = max(specularFactor, 0.0);
		float dampedFactor = pow(specularFactor, 1.0/* shineDamper */);
		totalSpecular = totalSpecular + (dampedFactor * 0.0 /* reflectivity */ * nsBlock.lights[i].color.xyz);
		
	}
	//totalDiffuse = max(totalDiffuse, 0.2);
	totalDiffuse = max(totalDiffuse * shadowFactor, 0.2);
	
	out_Color = vec4(totalDiffuse, 1.0) * texColor + vec4(totalSpecular, 1.0);
	
	// fogColor
	out_Color = mix(vec4(nsBlock.fogColor.xyz, 1.0), out_Color, visibility);
	
	//out_Color = vec4(totalDiffuse, 1.0) * texColor;
	//out_Color = texColor;
	
	//vec3 toneMapped = vec3(1.0) - exp(-out_Color.rgb * exposure);
	
	//out_Color.rgb = pow(toneMapped, vec3(1.0f / gamma));
	
}
