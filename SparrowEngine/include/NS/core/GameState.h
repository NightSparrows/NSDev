#pragma once

#include <string>

#include "Base.h"
#include "io/Time.h"
#include "io/Event.h"

namespace ns {

	///////////////////////////////////////////////////////////
	/// See this header file for more information.
	/// READ!!!
	///////////////////////////////////////////////////////////
	class NS_API GameState {

	private:
		std::string name;		// The string of the state

	public:
		GameState(std::string name) {
			this->name = name;
		}

		std::string getName() const { return this->name; }

		/////////////////////////////////////////////////////////////
		/// Remember to deal with the SFML event(當然你不要處裡也沒差)
		/////////////////////////////////////////////////////////////
		virtual void onEvent(ns::Event &evnt) { };

		virtual void onUpdate(Time delta) { };

		/////////////////////////////////////////////////////////////
		/// NS engine won't render automatically you must call
		/// GameEngine::Render() to render the process things
		/////////////////////////////////////////////////////////////
		virtual void onRender() { };

		virtual void init() { };

		virtual void cleanUp() { };

		////////////////////////////////////////////////////////////////
		/// changing state the current state will pause.(will call this)
		////////////////////////////////////////////////////////////////
		virtual void onPause() { };

		/////////////////////////////////////////////////////////////////
		/// on changing state prepare to change state will call this
		/////////////////////////////////////////////////////////////////
		virtual void onChange() { };

	};

}

