
#include "OpenGLTexture.h"

namespace ns {

	OpenGLTexture::OpenGLTexture(Type type) : Texture(type) {
		switch (type) {
		case Texture::texture2d:
			this->m_Target = GL_TEXTURE_2D;
			break;
		case Texture::textureCube:
			this->m_Target = GL_TEXTURE_CUBE_MAP;
			break;
		default:
			break;
		}
		glGenTextures(1, &this->m_ID);
	}

	OpenGLTexture::~OpenGLTexture() {
		glDeleteTextures(1, &this->m_ID);
	}

	/// 這樣就好，懶得判定
	OpenGLTexture::OpenGLTexture(Type type, GLuint id) : Texture(type) {
		this->m_ID = id;
		this->m_Target = GL_TEXTURE_2D;
	}

	void OpenGLTexture::bind() {
		glBindTexture(this->m_Target, this->m_ID);
	}

	void OpenGLTexture::unbind() {
		glBindTexture(this->m_Target, 0);
	}

	void OpenGLTexture::bind(uint32_t index) {
		// in some machine this just buggy
		//glBindTextureUnit(index, this->m_ID);
		glActiveTexture(GL_TEXTURE0 + index);
		this->bind();
	}

}