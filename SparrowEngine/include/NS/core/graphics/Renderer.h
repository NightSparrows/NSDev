#pragma once

#include "../Base.h"
#include <string>

namespace ns {

	/**
	 *
	 * Your Renderer class need to be abstracted.
	 * See virtual method to know how it works.
	 */
	class Renderer {
	public:
		Renderer(const std::string& name) : m_Name(name) { }
		virtual ~Renderer() { }

		/**
		 * The renderer which to be Add to the MasterRenderer it will
		 * initialize. (So you can create the renderer instance When engine is not created.)
		 */
		virtual void init() { };

		/// Updating method that is used on every frame
		virtual void update(float delta) { };

		/// The rendering method
		virtual void render() = 0;

		virtual void clearBuffer() { };

		virtual void cleanUp() { };

		std::string getName() { return this->m_Name; }

	private:
		std::string m_Name;

	};

}
