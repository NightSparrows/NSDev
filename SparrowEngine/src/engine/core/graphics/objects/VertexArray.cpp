
#include <NS/core/Log.h>
#include <NS/core/graphics/objects/VertexArray.h>
#include <platform/OpenGL/objects/OpenGLVertexArray.h>

namespace ns {

	Ref<VertexArray> VertexArray::Create() {
		return CreateRef<OpenGLVertexArray>();
	}

	VertexArray::VertexArray() {
		this->m_DataBuffer = Buffer::Create(Buffer::VertexBuffer);
		this->m_IndicesBuffer = Buffer::Create(Buffer::IndicesBuffer);
	}

	VertexArray::~VertexArray() {
#ifdef NS_DEBUG
		if (this->m_DataBuffer.use_count() > 1 || this->m_IndicesBuffer.use_count() > 1)
			NS_CORE_WARN("Data buffer or indices buffer are not unused while the parent vertex array will be released.");
#endif // NS_DEBUG
	}

}
