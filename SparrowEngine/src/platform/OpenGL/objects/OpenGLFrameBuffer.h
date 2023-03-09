#pragma once

#include <glad/glad.h>
#include <NS/core/graphics/objects/FrameBuffer.h>

namespace ns {

	class NS_API OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(Type type, uint32_t width, uint32_t height, uint32_t buffers);
		~OpenGLFrameBuffer();

		void bindOnly() override;
		void unbindOnly() override;

		void resolveToFbo(Ref<FrameBuffer> outputFbo) override;
		void resolveToFbo(uint32_t readBuffer, uint32_t drawBuffer, Ref<FrameBuffer> outputFbo) override;

		void resolveToScreen(uint32_t attachment) override;

		void attachCubeMapTexture(Ref<Texture> cubeMapTexture, int face) override;
		void drawAttachment(int index) override;

	private:
		void createDepthRenderBuffer(bool multisamples);
		void createColorRenderBuffer(int buffers);

		void createDepthTexture();
		void createColorTextures(int buffers);

		////////////////////////////////////////////////////////////////////////
		/// Because color render buffer is only use in multisample
		///	so no need to create none multisample render buffer
		/// ¤j·§...
		////////////////////////////////////////////////////////////////////////
		void createMultisampleColorAttachment(GLenum attachment);

	private:
		GLuint m_ID;

	};


}
