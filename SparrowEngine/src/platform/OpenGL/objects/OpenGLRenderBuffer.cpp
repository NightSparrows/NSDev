
#include "OpenGLRenderBuffer.h"

namespace ns {



	OpenGLRenderBuffer::OpenGLRenderBuffer(uint32_t width, uint32_t height) :
		RenderBuffer(width, height)
	{
		glGenRenderbuffers(1, &this->m_ID);
	}

	void OpenGLRenderBuffer::attachToFbo(uint32_t attachment) {
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, this->m_ID);
	}

	void OpenGLRenderBuffer::storage(uint32_t internalFormat) {
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, this->m_Width, this->m_Height);
	}

	void OpenGLRenderBuffer::storageMultisample(uint32_t samples, uint32_t internalFormat) {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, this->m_Width, this->m_Height);
	}

	void OpenGLRenderBuffer::bind() {
		glBindRenderbuffer(GL_RENDERBUFFER, this->m_ID);
	}

	void OpenGLRenderBuffer::unbind() {
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	OpenGLRenderBuffer::~OpenGLRenderBuffer() {
		glDeleteRenderbuffers(1, &this->m_ID);
	}
}
