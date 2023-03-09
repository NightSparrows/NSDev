
#include "platform/OpenGL/objects/OpenGLTexture.h"

namespace ns {

	Ref<Texture> Texture::NO_TEXTURE = CreateRef<OpenGLTexture>(texture2d, 0);

	Texture::Texture(Texture::Type type) {
		this->m_Type = type;
	}

	Texture::Type Texture::getType() {
		return this->m_Type;
	}

	Ref<Texture> Texture::Create(Type type) {
		return CreateRef<OpenGLTexture>(type);
	}

}
