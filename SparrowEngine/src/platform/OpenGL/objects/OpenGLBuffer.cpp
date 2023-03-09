#include <glad/glad.h>
#include <NS/core/Log.h>
#include "OpenGLBuffer.h"

namespace ns {

	OpenGLBuffer::OpenGLBuffer(Buffer::Type type) {
		switch (type) {
		case Buffer::VertexBuffer:
		{
			this->m_Target = GL_ARRAY_BUFFER;
			break;
		}
		case Buffer::IndicesBuffer:
		{
			this->m_Target = GL_ELEMENT_ARRAY_BUFFER;
			break;
		}
		case Buffer::UniformBuffer:
			this->m_Target = GL_UNIFORM_BUFFER;
			break;
		default:
			NS_CORE_ASSERT(false, "Unknown buffer type");
			break;
		}
		glGenBuffers(1, &this->m_ID);
	}

	OpenGLBuffer::~OpenGLBuffer() {
		glDeleteBuffers(1, &this->m_ID);
	}

	void OpenGLBuffer::bind() {
		glBindBuffer(this->m_Target, this->m_ID);
	}

	void OpenGLBuffer::unbind() {
		glBindBuffer(this->m_Target, 0);
	}

	void OpenGLBuffer::storeData(void *data, uint32_t size, Usage usage) {
		glBufferData(this->m_Target, size, data, getUsage(usage));
	}

	void OpenGLBuffer::emptyData(uint32_t size, Usage usage) {
		glBufferData(this->m_Target, size, nullptr, getUsage(usage));
	}

	uint32_t OpenGLBuffer::getUsage(Usage usage) {
		GLenum usageData = GL_STATIC_DRAW;
		switch (usage) {
		case Buffer::StaticDraw:
			break;
		case Buffer::DynamicDraw:
			usageData = GL_DYNAMIC_DRAW;
			break;
		case Buffer::StreamDraw:
			usageData = GL_STREAM_DRAW;
			break;
		default:
			NS_CORE_ASSERT(false, "Unknown buffer usage type");
			break;
		}
		return usageData;
	}

	void *OpenGLBuffer::startMapping() {
		return glMapBuffer(this->m_Target, GL_READ_WRITE | GL_MAP_UNSYNCHRONIZED_BIT);
	}

	void OpenGLBuffer::stopMapping() {
		glUnmapBuffer(this->m_Target);
	}

	void OpenGLBuffer::bindBase(uint32_t index) {
		glBindBufferBase(this->m_Target, index, this->m_ID);
	}
}