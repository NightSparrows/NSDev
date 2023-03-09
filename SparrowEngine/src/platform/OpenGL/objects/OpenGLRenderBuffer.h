#pragma once

#include <glad/glad.h>
#include <NS/core/graphics/objects/RenderBuffer.h>

namespace ns {

	class NS_API OpenGLRenderBuffer : public RenderBuffer {

	public:
		OpenGLRenderBuffer(uint32_t width, uint32_t height);
		~OpenGLRenderBuffer();


		void bind();
		void unbind();

		void attachToFbo(uint32_t attachment);

		void storage(uint32_t internalFormat);

		void storageMultisample(uint32_t samples, uint32_t internalFormat);

	private:
		GLuint m_ID;
	};

}
