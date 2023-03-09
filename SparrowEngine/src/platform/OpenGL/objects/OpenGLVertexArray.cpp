

#include <glad/glad.h>
#include "OpenGLVertexArray.h"

namespace ns {

	OpenGLVertexArray::OpenGLVertexArray() {
		glGenVertexArrays(1, &this->m_ID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &this->m_ID);
	}

	void OpenGLVertexArray::bind() const {
		glBindVertexArray(this->m_ID);
	}

	void OpenGLVertexArray::unbind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::enableIndex(uint32_t index) {
		glEnableVertexArrayAttrib(this->m_ID, index);
	}

	void OpenGLVertexArray::disableIndex(uint32_t index) {
		glDisableVertexArrayAttrib(this->m_ID, index);
	}
}