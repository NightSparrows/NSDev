
#include "Skybox.h"

Skybox::Skybox() {
	this->m_texture = ns::Texture::Create(ns::Texture::textureCube);
}

Skybox::~Skybox() {
	this->m_texture.reset();
}


void Skybox::loadTexture(std::vector<std::string>* texturesFilePath) {
	ns::Loader::loadTextureCube(this->m_texture, texturesFilePath);
}
