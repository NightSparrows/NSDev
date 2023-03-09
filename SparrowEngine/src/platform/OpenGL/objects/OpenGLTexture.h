#pragma once

#include <glad/glad.h>
#include <NS/core/graphics/objects/Texture.h>

namespace ns {

	class OpenGLFrameBuffer;

	class NS_API OpenGLTexture : public Texture {
		friend class OpenGLFrameBuffer;
	public:
		OpenGLTexture(Type type);
		~OpenGLTexture();

		void bind() override;
		void unbind() override;

		void bind(uint32_t index) override;

		OpenGLTexture(Type type, GLuint id);

	private:
		GLuint m_ID;
		GLenum m_Target;
	};
}
