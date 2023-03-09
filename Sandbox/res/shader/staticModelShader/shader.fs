#version 330

in vec2 vs_texCoord;

layout (location = 0) out vec4 out_Color;

uniform sampler2D textureSampler;

void main() {

	vec4 texColor = texture(textureSampler, texCoord);
	
	out_Color = texColor;
	
}
