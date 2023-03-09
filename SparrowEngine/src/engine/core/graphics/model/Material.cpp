
#include <NS/core/graphics/model/Material.h>

namespace ns {

	Material Material::NO_MATERIAL = Material();

	Material::Material() {
		this->m_diffuseTexture = Texture::NO_TEXTURE;
		this->m_normalTexture = Texture::NO_TEXTURE;
		this->m_extraInfoTexture = Texture::NO_TEXTURE;
		this->m_hasExtraInfoTexture = false;
		this->m_hasNormalTexture = false;
		this->m_ambient = glm::vec3(1);
		this->m_diffuse = glm::vec4(1);
	}

	Material::~Material() {
	}

	Ref<Texture> Material::getDiffues() {
		return this->m_diffuseTexture;
	}

	void Material::setDiffuse(Ref<Texture> texture) {
		this->m_diffuseTexture = texture;
	}

	bool Material::hasDiffuseTexture() {
		return this->m_diffuseTexture != Texture::NO_TEXTURE;
	}

	Ref<Texture> Material::getNormalTexture() {
		return this->m_normalTexture;
	}

	void Material::setNormalTexture(Ref<Texture> texture) {
		this->m_normalTexture = texture;
		this->m_hasNormalTexture = true;
	}

	bool Material::hasNormalTexture() { return this->m_hasNormalTexture; }

	Ref<Texture> Material::getExtraInfoTexture() {
		return this->m_extraInfoTexture;
	}

	void Material::setExtraInfoTexture(Ref<Texture> texture) {
		this->m_extraInfoTexture = texture;
		this->m_hasExtraInfoTexture = true;
	}

	bool Material::hasExtraInfoTexture() { return this->m_hasExtraInfoTexture; }

	glm::vec3 Material::getAmbient() {
		return this->m_ambient;
	}

	void Material::setAmbient(float r, float g, float b) {
		this->m_ambient.r = r;
		this->m_ambient.g = g;
		this->m_ambient.b = b;
	}

	glm::vec4 Material::getDiffuseColor() {
		return this->m_diffuse;
	}

	void Material::setDiffuseColor(float r, float g, float b, float a) {
		this->m_diffuse.r = r;
		this->m_diffuse.g = g;
		this->m_diffuse.b = b;
		this->m_diffuse.a = a;
	}

}
