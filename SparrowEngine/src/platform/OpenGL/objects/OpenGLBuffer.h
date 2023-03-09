#pragma once

#include <NS/core/graphics/objects/Buffer.h>

namespace ns {

	class NS_API OpenGLBuffer : public Buffer {
	public:
		OpenGLBuffer(Type type);
		~OpenGLBuffer();

		void bind() override;
		void unbind() override;

		void storeData(void *data, uint32_t size, Usage usage) override;

		void emptyData(uint32_t size, Usage usage) override;

		void *startMapping() override;
		void stopMapping() override;

		void bindBase(uint32_t index) override;

	private:
		uint32_t getUsage(Usage usage);

	private:
		uint32_t m_ID;
		uint32_t m_Target;
	};

}
