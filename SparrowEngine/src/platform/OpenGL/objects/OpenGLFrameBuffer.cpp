
#include "OpenGLFrameBuffer.h"
#include "OpenGLTexture.h"
#include <NS/core/Log.h>
#include <NS/core/GameEngine.h>

namespace ns {

	void OpenGLFrameBuffer::bindOnly() {
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_ID);
	}

	void OpenGLFrameBuffer::unbindOnly() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::resolveToFbo(Ref<FrameBuffer> outputFbo) {
		// TODO: check this if it works.
		Ref<OpenGLFrameBuffer> outputFboCast = std::dynamic_pointer_cast<OpenGLFrameBuffer>(outputFbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, outputFboCast->m_ID);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->m_ID);
		glBlitFramebuffer(0, 0, this->m_Width, this->m_Height, 0, 0, outputFboCast->m_Width, outputFboCast->m_Height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		this->unbindOnly();
	}

	void OpenGLFrameBuffer::resolveToFbo(uint32_t readBuffer, uint32_t drawBuffer, Ref<FrameBuffer> outputFbo) {
		// TODO: check this if it works.
		Ref<OpenGLFrameBuffer> outputFboCast = std::dynamic_pointer_cast<OpenGLFrameBuffer>(outputFbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, outputFboCast->m_ID);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->m_ID);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + readBuffer);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + drawBuffer);
		glBlitFramebuffer(0, 0, this->m_Width, this->m_Height, 0, 0, outputFboCast->m_Width, outputFboCast->m_Height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_DRAW_BUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	}

	void OpenGLFrameBuffer::resolveToScreen(uint32_t attachment) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->m_ID);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
		glDrawBuffer(GL_BACK);
		glBlitFramebuffer(0, 0, this->m_Width, this->m_Height, 0, 0, ns::GameEngine::GetWindow()->getWidth(), ns::GameEngine::GetWindow()->getHeight(),
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::attachCubeMapTexture(Ref<Texture> cubeMapTexture, int face) {
		Ref<OpenGLTexture> texCasted = std::dynamic_pointer_cast<OpenGLTexture>(cubeMapTexture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, texCasted->m_ID, 0);
	}

	void OpenGLFrameBuffer::drawAttachment(int index) {
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + index);
	}

	void OpenGLFrameBuffer::createDepthRenderBuffer(bool multisamples)
	{
		this->m_DepthBuffer = RenderBuffer::Create(this->m_Width, this->m_Height);
		this->m_DepthBuffer->bind();
		if (multisamples) {
			this->m_DepthBuffer->storageMultisample(4/* can configurable */, GL_DEPTH_COMPONENT24);
		}
		else {
			this->m_DepthBuffer->storage(GL_DEPTH_COMPONENT24);
		}
		this->m_DepthBuffer->attachToFbo(GL_DEPTH_ATTACHMENT);
	}

	void OpenGLFrameBuffer::createColorRenderBuffer(int buffers)
	{
		for (int i = 0; i < buffers; i++) {
			this->createMultisampleColorAttachment(GL_COLOR_ATTACHMENT0 + i);
		}
	}

	void OpenGLFrameBuffer::createDepthTexture()
	{
		this->m_DepthTexture = Texture::Create(Texture::texture2d);
		this->m_DepthTexture->bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, this->m_Width, this->m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		Ref<OpenGLTexture> texCasted = std::dynamic_pointer_cast<OpenGLTexture>(this->m_DepthTexture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texCasted->m_ID, 0);
		//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texCasted->m_ID, 0/* minimap level*/);
	}

	void OpenGLFrameBuffer::createColorTextures(int buffers)
	{
		for (int i = 0; i < buffers; i++) {
			Ref<Texture> tex = Texture::Create(Texture::texture2d);
			tex->bind();
			// 不清楚GL_RGB跟GL_RGB8有甚麼差
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->m_Width, this->m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			Ref<OpenGLTexture> texCasted = std::dynamic_pointer_cast<OpenGLTexture>(tex);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, texCasted->m_ID, 0/* minimap level*/);
			this->m_Textures.push_back(tex);			// 因為一定會造順序
		}
	}

	void OpenGLFrameBuffer::createMultisampleColorAttachment(GLenum attachment)
	{
		Ref<RenderBuffer> colorBuffer = RenderBuffer::Create(this->m_Width, this->m_Height);
		this->m_ColorBuffers.push_back(colorBuffer);
		colorBuffer->bind();
		colorBuffer->storageMultisample(4, GL_RGB8);
		colorBuffer->attachToFbo(attachment);
		colorBuffer->unbind();
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(Type type, uint32_t width, uint32_t height, uint32_t buffers) :
		FrameBuffer(width, height)
	{
		glGenFramebuffers(1, &this->m_ID);

		this->m_DepthTexture = Texture::NO_TEXTURE;

		this->bindOnly();

		GLenum *bufferPtr = new GLenum[buffers];
		//std::vector<GLenum> bufferList(buffers);
		for (uint32_t i = 0; i < buffers; i++) {
			bufferPtr[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(buffers, bufferPtr);
		delete bufferPtr;

		switch (type)
		{
		case ns::FrameBuffer::None:
			break;
		case ns::FrameBuffer::DepthTexture:
			this->createDepthTexture();
			this->createColorTextures(buffers);
			break;
		case ns::FrameBuffer::DepthRenderBuffer:
			this->createDepthRenderBuffer(false);
			this->createColorTextures(buffers);
			break;
		case ns::FrameBuffer::MultiSamples:
			this->createDepthRenderBuffer(true);
			this->createColorRenderBuffer(buffers);
			break;
		default:
			break;
		}
		NS_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is incomplete");

		this->unbindOnly();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
#ifdef NS_DEBUG
		if (this->m_DepthBuffer.use_count() > 1) {
			NS_CORE_WARN("Depth buffer is not unused while Fbo({0}) will be released.", this->m_ID);
		}

		if (this->m_DepthTexture.use_count() > 1) {
			NS_CORE_WARN("Depth texture is not unused while Fbo({0}) will be released.", this->m_ID);
		}
		for (Ref<RenderBuffer> colorBuffer : this->m_ColorBuffers) {
			if (colorBuffer.use_count() > 1) {
				NS_CORE_WARN("Color Rbo is not unused while Fbo({0}) will be released.", this->m_ID);
			}
		}
		for (Ref<Texture> texture : this->m_Textures) {
			if (texture.use_count() > 1) {
				NS_CORE_WARN("Color texture is not unused while Fbo({0}) will be released.", this->m_ID);
			}
		}
#endif // NS_DEBUG
		this->m_DepthBuffer.reset();
		this->m_DepthTexture.reset();
		for (Ref<Texture> colorTexture : this->m_Textures) {
			colorTexture.reset();
		}
		for (Ref<RenderBuffer> colorBuffer : this->m_ColorBuffers) {
			colorBuffer.reset();
		}
		glDeleteFramebuffers(1, &this->m_ID);
	}

}
