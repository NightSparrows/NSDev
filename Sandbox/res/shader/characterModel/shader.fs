#version 330

in vec2 texCoord;

out vec4 out_Color;

uniform sampler2D textureSampler;

void main() {


	out_Color = texture(textureSampler, texCoord);
	
}