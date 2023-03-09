#version 440

in vec2 texCoord;

out vec4 out_Color;

layout (location = 0) uniform sampler2D tex_gPosition;
layout (location = 1) uniform sampler2D tex_gNormal;
layout (location = 2) uniform sampler2D tex_gColor;

const int MAX_LIGHTS = 8;

struct Light {
	vec4 position;
	vec4 color;
	vec4 attenuation;
};

layout(std140) uniform engineUbo {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 fogColor;
	vec4 fogProperty;
	Light lights[MAX_LIGHTS];
}nsBlock;

const float levels = 5000.0;

void main() {

	vec3 fragPos = texture(tex_gPosition, texCoord).rgb;
	vec3 normal = texture(tex_gNormal, texCoord).rgb;
	vec4 color = texture(tex_gColor, texCoord);

	vec3 totalDiffuse = vec3(0);

	for (int i = 0; i < MAX_LIGHTS; i++) {
		vec3 toLightVector = nsBlock.lights[i].position.xyz - fragPos;
		vec3 unitLightVector = normalize(toLightVector);
		
		float distance = length(toLightVector);
		float attFactor = nsBlock.lights[i].attenuation.x + (nsBlock.lights[i].attenuation.y * distance) + (nsBlock.lights[i].attenuation.z * distance * distance);
		
		float brightness = dot(normal, unitLightVector);
		brightness = brightness / attFactor;
		brightness = clamp(brightness, 0.0, 1.0);
		float level = floor(brightness * levels);
		
		vec3 diffuse;
		
		if (level < 3) {
			diffuse = ((nsBlock.lights[i].color.xyz * 0.2) + vec3(0.8)) / MAX_LIGHTS;
			diffuse = max(diffuse, 0.0);
			totalDiffuse += diffuse;
		} else {
			diffuse = ((0.2 * nsBlock.lights[i].color.xyz) + vec3(0.85)) / MAX_LIGHTS;
			diffuse = max(diffuse, 0.0);
			totalDiffuse += diffuse;
		}
		
	}

	totalDiffuse = max(totalDiffuse, 0.2);
	
	out_Color = vec4(totalDiffuse, 1.0) * color;
	// out_Color = color;
	// out_Color = vec4(-fragPos, 1.0);
	
}
