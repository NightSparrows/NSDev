#include "Terrain.h"

Terrain::Terrain(int x, int z) {
	this->m_x = x;
	this->m_z = z;
	this->m_blendTexture = ns::Texture::Create(ns::Texture::texture2d);
	this->m_rTexture = ns::Texture::Create(ns::Texture::texture2d);
	this->m_gTexture = ns::Texture::Create(ns::Texture::texture2d);
	this->m_bTexture = ns::Texture::Create(ns::Texture::texture2d);
	this->m_bgTexture = ns::Texture::Create(ns::Texture::texture2d);
}

Terrain::~Terrain() {
	this->m_blendTexture.reset();
	this->m_rTexture.reset();
	this->m_gTexture.reset();
	this->m_bTexture.reset();
	this->m_bgTexture.reset();
}

void Terrain::bind() {
	this->m_blendTexture->bind(0);
	this->m_rTexture->bind(1);
	this->m_gTexture->bind(2);
	this->m_bTexture->bind(3);
	this->m_bgTexture->bind(4);
}

int Terrain::getX() {
	return this->m_x;
}

int Terrain::getZ() {
	return this->m_z;
}

ns::Ref<ns::Texture> Terrain::getBlendMap()
{
	return this->m_blendTexture;
}

ns::Ref<ns::Texture> Terrain::getrTexture() {
	return this->m_rTexture;
}

ns::Ref<ns::Texture> Terrain::getgTexture() {
	return this->m_gTexture;
}

ns::Ref<ns::Texture> Terrain::getbTexture() {
	return this->m_bTexture;
}

ns::Ref<ns::Texture> Terrain::getbgTexture() {
	return this->m_bgTexture;
}
