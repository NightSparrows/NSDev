

#include <glad/glad.h>
#include <NS/core/graphics/RenderAPI.h>


namespace ns {

	void RenderAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void RenderAPI::ClearColor(glm::vec4 color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderAPI::Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	void RenderAPI::DrawArrays(Shape shape, uint32_t first, uint32_t count) {

		GLenum mode;
		switch (shape)
		{
		case ns::RenderAPI::Triangle:
			mode = GL_TRIANGLES;
			break;
		case ns::RenderAPI::TriangleStrip:
			mode = GL_TRIANGLE_STRIP;
			break;
		default:
			break;
		}
		glDrawArrays(mode, first, count);
	}

	void RenderAPI::DrawIndexed(uint32_t indicesCount, uint32_t offset) {
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	}

	void RenderAPI::DrawIndexInstanced(uint32_t indicesCount, uint32_t offset, uint32_t count) {
		glDrawElementsInstanced(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)), count);
	}

	void RenderAPI::AttributeFloatPointer(uint32_t index, uint32_t size, bool normalized, uint32_t stride, const void *pointer) {
		glVertexAttribPointer(index, size, GL_FLOAT, normalized ? GL_TRUE : GL_FALSE, stride, pointer);
	}

	void RenderAPI::AttributeDivisor(uint32_t index, uint32_t divisor) {
		glVertexAttribDivisor(index, divisor);
	}

	void RenderAPI::AttributeIntegerPointer(uint32_t index, uint32_t size, uint32_t stride, const void *pointer) {
		glVertexAttribIPointer(index, size, GL_INT, stride, pointer);
	}

	void RenderAPI::EnableVertexAttributes(uint32_t count) {
		for (uint32_t i = 0; i < count; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void RenderAPI::DisableVertexAttributes(uint32_t count) {
		for (uint32_t i = 0; i < count; i++) {
			glDisableVertexAttribArray(i);
		}
	}

	void RenderAPI::PushGLSettings() {
		
	}

	void RenderAPI::PopGLSettings() {

	}

	void RenderAPI::Blend(bool value) {
		if (value) {
			glEnable(GL_BLEND);
		}
		else {
			glDisable(GL_BLEND);
		}
	}

	void RenderAPI::DepthTest(bool value) {
		if (value) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}
	}

	void RenderAPI::DepthMask(bool value) {
		glDepthMask(value ? GL_TRUE : GL_FALSE);
	}

	void RenderAPI::CullFace(bool value) {
		if (value) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);			// usually just back
		}
		else {
			glDisable(GL_CULL_FACE);
		}
	}
}
