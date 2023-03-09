#pragma once

#include "../../deps/glm/glm.hpp"
#include "../objects/Texture.h"

namespace ns {

	///////////////////////////////////////////////////////////////
	/// Model Texture material
	/// Default set all textures to Texture::NO_TEXTURE
	/// Delete will release all texture it has bound.
	///////////////////////////////////////////////////////////////
	class NS_API Material {
	public:
		Material();
		~Material();

		Ref<Texture> getDiffues();

		void setDiffuse(Ref<Texture> texture);
		bool hasDiffuseTexture();

		Ref<Texture> getNormalTexture();
		void setNormalTexture(Ref<Texture> texture);
		bool hasNormalTexture();

		Ref<Texture> getExtraInfoTexture();
		void setExtraInfoTexture(Ref<Texture> texture);
		bool hasExtraInfoTexture();

		// Àô¹Ò¥ú
		glm::vec3 getAmbient();
		void setAmbient(float r, float g, float b);

		// diffuse lighting
		glm::vec4 getDiffuseColor();
		void setDiffuseColor(float r, float g, float b, float a);

		static Material NO_MATERIAL;

	private:
		Ref<Texture> m_diffuseTexture;
		Ref<Texture> m_normalTexture;			// normal mapping 
		Ref<Texture> m_extraInfoTexture;

		bool m_hasNormalTexture;
		bool m_hasExtraInfoTexture;

		glm::vec3 m_ambient;
		glm::vec4 m_diffuse;
		glm::vec3 m_specular;

		float m_shineDamper;
		float m_reflectivity;

	};

}
