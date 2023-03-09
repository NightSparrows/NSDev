#pragma once

#include "../Base.h"
#include "../deps/glm/glm.hpp"

namespace ns {

	// in OpenGL RenderAPI (OpenGLRenderAPI.cpp)
	// The render API that use for rendering
	class RenderAPI {
	public:
		enum Shape {
			Triangle,
			TriangleStrip
		};

		static void Clear();

		static void ClearColor(glm::vec4 color);

		static void Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void DrawArrays(Shape shape, uint32_t first, uint32_t count);

		static void DrawIndexed(uint32_t indicesCount, uint32_t offset);

		// \param indicesCount
		//		How many indices to draw in one instance
		// \param offset
		//		the offset of the model
		// \param count
		//		How many instance to draw
		static void DrawIndexInstanced(uint32_t indicesCount, uint32_t offset, uint32_t count);

		static void AttributeFloatPointer(uint32_t index, uint32_t size, bool normalized, uint32_t stride, const void *pointer);

		static void AttributeDivisor(uint32_t index, uint32_t divisor);

		static void AttributeIntegerPointer(uint32_t index, uint32_t size, uint32_t stride, const void *pointer);

		static void EnableVertexAttributes(uint32_t count);

		static void DisableVertexAttributes(uint32_t count);

		static void PushGLSettings();

		static void PopGLSettings();

		// Some settings
		static void Blend(bool value);
		static void DepthTest(bool value);
		static void DepthMask(bool value);		// whether it should render to depth buffer, false mean not going to have depth
		static void CullFace(bool value);

	};

}

