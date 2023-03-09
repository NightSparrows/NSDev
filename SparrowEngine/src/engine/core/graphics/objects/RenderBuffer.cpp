
#include <NS/core/graphics/objects/RenderBuffer.h>
#include <platform/OpenGL/objects/OpenGLRenderBuffer.h>

namespace ns {

	Ref<RenderBuffer> RenderBuffer::Create(uint32_t width, uint32_t height) {
		return CreateRef<OpenGLRenderBuffer>(width, height);
	}

	RenderBuffer::RenderBuffer(uint32_t width, uint32_t height) :
		m_Width(width), m_Height(height)
	{}

}
