#pragma once

#include "../../Base.h"

namespace ns {

	// OpenGL Buffer Object
	class NS_API Buffer {
	public:
		enum Type
		{
			VertexBuffer,
			IndicesBuffer,
			UniformBuffer
		};

		enum Usage
		{
			StaticDraw,
			DynamicDraw,
			StreamDraw
		};

	public:
		virtual ~Buffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		/// \param size
		/// 	per byte
		virtual void storeData(void *data, uint32_t size, Usage usage) = 0;

		/// \param size
		/// 	per byte
		virtual void emptyData(uint32_t size, Usage usage) = 0;

		///////////
		/// Memory mapping
		virtual void *startMapping() = 0;
		virtual void stopMapping() = 0;

		/// OpenGL:
		/// 	Used to bind like Uniform Buffer binding.
		virtual void bindBase(uint32_t index) = 0;

		static Ref<Buffer> Create(Type type);

	};

}
