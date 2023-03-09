
#include <NS/core/Log.h>
#include <NS/core/graphics/objects/FrameBuffer.h>
#include <platform/OpenGL/objects/OpenGLFrameBuffer.h>
#include <NS/core/graphics/RenderAPI.h>
#include <NS/core/GameEngine.h>

namespace ns {

	Ref<FrameBuffer> FrameBuffer::Create(Type type, uint32_t width, uint32_t height, uint32_t buffers) {
		return CreateRef<OpenGLFrameBuffer>(type, width, height, buffers);
	}

	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height) :
		m_Width(width), m_Height(height)
	{ }

	FrameBuffer::~FrameBuffer() {
#ifdef NS_DEBUG
		if (this->m_DepthTexture.use_count() > 1)
			NS_CORE_WARN("Depth Texture is not unused while FrameBuffer will be released.");
		for (Ref<Texture> texture : this->m_Textures) {
			if (texture.use_count() > 1)
				NS_CORE_WARN("Color texture is not unused while Framebuffer will be released.");
		}
#endif // NS_DEBUG
	}

	void FrameBuffer::bind() {
		this->bindOnly();
		RenderAPI::Viewport(0, 0, this->m_Width, this->m_Height);
	}

	void FrameBuffer::unbind() {
		this->unbindOnly();
		RenderAPI::Viewport(0, 0, ns::GameEngine::GetWindow()->getWidth(), ns::GameEngine::GetWindow()->getHeight());
	}

	Ref<Texture> FrameBuffer::getDepthTexture() {
		return this->m_DepthTexture;
	}

	Ref<Texture> FrameBuffer::getColorTexture(uint32_t attachment) {
		if (attachment < this->m_Textures.size()) {
			return this->m_Textures[attachment];
		}
		else {
			NS_CORE_ERROR("Attachment {0} is not in this Fbo!", attachment);
			return NULL;
		}
	}
}
