#version 330

// in
in vec3 texCoord;

// out
layout (location = 0) out vec4 out_Color;

// uniform
uniform samplerCube skyboxTexture;

void main() {

	out_Color = texture(skyboxTexture, texCoord);
	
}
