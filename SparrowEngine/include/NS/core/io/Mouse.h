#pragma once


#ifndef NSGAMEENGINE_MOUSE_H
#define NSGAMEENGINE_MOUSE_H

#include "../Base.h"

#include "../deps/glm/glm.hpp"
#include "../GameEngine.h"

namespace ns {

	/**
	* Add abstract method in each platform
	* Windows: WindowsMouse
	*/
	class NS_API Mouse {
	public:
		// from glfw3.h
		enum Button {
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Button6 = 6,
			Button7 = 7,
			ButtonLast = Button7,
			Left = Button0,
			Right = Button1,
			Middle = Button2
		};

		static bool IsMouseButtonDown(Button button);

		static glm::vec2 GetPosition();

		/// Set position relativily to window.
		static void SetPosition(int x, int y);

		static float GetDX();
		static float GetDY();

		/// Grab the mouse
		/// if the platform is GLFW
		/// Then Grab the mouse will provides you like minecraft mouse movement
		static void Grab(bool grab);

		static bool IsGrab();
	};

}

#endif //NSGAMEENGINE_MOUSE_H

