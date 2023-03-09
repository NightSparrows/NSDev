#pragma once

#include "../../Base.h"

namespace ns {

	// OpenGL render buffer
	class NS_API RenderBuffer {
	public:
		virtual ~RenderBuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void attachToFbo(uint32_t attachment) = 0;

		virtual void storage(uint32_t internalFormat) = 0;

		virtual void storageMultisample(uint32_t samples, uint32_t internalFormat) = 0;

		static Ref<RenderBuffer> Create(uint32_t width, uint32_t height);

	protected:
		RenderBuffer(uint32_t width, uint32_t height);

	protected:
		uint32_t m_Width, m_Height;
	};

}
