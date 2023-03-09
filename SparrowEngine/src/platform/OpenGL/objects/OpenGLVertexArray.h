#pragma once

#include <NS/core/graphics/objects/VertexArray.h>

namespace ns {

	class NS_API OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void bind() const override;
		void unbind() const override;

		void enableIndex(uint32_t index) override;
		void disableIndex(uint32_t index) override;

	private:
		uint32_t m_ID;
	};

}

