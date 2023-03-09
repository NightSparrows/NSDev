#pragma once

#include "Buffer.h"

namespace ns {

	class NS_API VertexArray {

	public:
		VertexArray();
		virtual ~VertexArray();

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		/// Enabling the vertex attribute index to this VAO
		/// \param index
		///		the index enabling bound for shader usage
		virtual void enableIndex(uint32_t index) = 0;
		virtual void disableIndex(uint32_t index) = 0;
		
		Ref<Buffer> getIndicesBuffer() { return this->m_IndicesBuffer; }
		Ref<Buffer> getDataBuffer() { return this->m_DataBuffer; }

		static Ref<VertexArray> Create();

	protected:
		Ref<Buffer> m_IndicesBuffer;
		Ref<Buffer> m_DataBuffer;
	};

}
