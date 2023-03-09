#pragma once

#include "../../Base.h"

namespace ns {

	struct NS_API TextureConfig {
	public:
		bool mipMapping = true;
		bool anisotropicFiltering = true;
	};

	class NS_API Texture {
	public:

		enum Type {
			texture2d,
			textureCube
		};

		Texture(Type type);
		virtual ~Texture() = default;

		/// You must define a NO_TEXTURE instance to represented
		/// See OpenGLTexture.cpp for example
		static Ref<Texture> NO_TEXTURE;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void bind(uint32_t index) = 0;

		Texture::Type getType();

		static Ref<Texture> Create(Type type);

	private:
		Type m_Type;

	};

}
