#pragma once

#include "../Base.h"

namespace ns {

	class NS_API GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void init() = 0;
		virtual void swapBuffers() = 0;
		virtual void cleanUp() {};

		/// 
		static Scope<GraphicsContext> Create(void *window);

	};

}
