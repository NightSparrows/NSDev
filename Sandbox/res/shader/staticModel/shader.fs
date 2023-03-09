#version 430

in vec2 texCoord;
in float visibility;
in vec3 fogColor;

in vec3 gPosition;
in vec3 gNormal;

layout (location = 0) out vec3 out_Position;
layout (location = 1) out vec3 out_Normal;
layout (location = 2) out vec4 out_Color;

uniform sampler2D textureSampler;

uniform vec3 u_ambient;
uniform vec4 u_diffuse;

void main() {

	vec4 texColor = texture(textureSampler, texCoord);
	/*
	texColor.r *= u_ambient.r;
	texColor.g *= u_ambient.g;
	texColor.b *= u_ambient.b;
	*/

	// texColor = texColor * u_diffuse;

	if (texColor.a < 0.1) {
		discard;
	}

	out_Color = texColor;
	// out_Color = mix(vec4(fogColor, 1.0), out_Color, visibility);
	
	
	out_Position = gPosition;
	out_Normal = gNormal;
}