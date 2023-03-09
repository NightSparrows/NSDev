#pragma once

#include <vector>

#include "Texture.h"
#include "RenderBuffer.h"

namespace ns {

	// TODO: Untested
	class NS_API FrameBuffer {
	public:
		enum Type
		{
			None = 0,
			DepthTexture,
			DepthRenderBuffer,
			MultiSamples
		};

		virtual ~FrameBuffer();

		virtual void bindOnly() = 0;
		void bind();

		virtual void unbindOnly() = 0;

		// unbind the Fbo and change the viewport to the main window size.
		void unbind();

		virtual void resolveToFbo(Ref<FrameBuffer> outputFbo) = 0;

		/// \param readBuffer, drawBuffer
		///		color atachment to resolve
#define DEPTH 32
		///		depth attachment is 32
		virtual void resolveToFbo(uint32_t readBuffer, uint32_t drawBuffer, Ref<FrameBuffer> outputFbo) = 0;

		virtual void resolveToScreen(uint32_t attachment) = 0;

		Ref<Texture> getDepthTexture();
		Ref<Texture> getColorTexture(uint32_t attachment);

		// you must set the color texture to 0 otherwise it will not work correctly
		virtual void attachCubeMapTexture(Ref<Texture> cubeMapTexture, int face) = 0;
		virtual void drawAttachment(int index) = 0;

		/// \param buffers
		///		How many buffer, a positive number
		static Ref<FrameBuffer> Create(Type type, uint32_t width, uint32_t height, uint32_t buffers);

	protected:
		FrameBuffer(uint32_t width, uint32_t height);

	protected:
		Ref<RenderBuffer> m_DepthBuffer;
		Ref<Texture> m_DepthTexture;
		std::vector<Ref<Texture>> m_Textures;
		std::vector<Ref<RenderBuffer>> m_ColorBuffers;

		uint32_t m_Width, m_Height;
	};
}
