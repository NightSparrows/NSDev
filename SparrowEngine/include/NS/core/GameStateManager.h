#pragma once

#include <vector>
#include "GameState.h"

namespace ns {

	//////////////////////////////////////////////////////////////////
	/// The game state manager
	/// for my engine to manage state
	/// Won't delete states after delete this class only cleanUp
	//////////////////////////////////////////////////////////////////
	class NS_API GameStateManager {

	private:
		std::vector<GameState*> states;

		GameState *currentState;
		GameState *nextState;

		bool shouldChangeState;

	public:
		GameStateManager();

		void init(GameState* startState);

		void addState(GameState* state);

		void changeState(const char *stateName);

		void onEvent(ns::Event& event);

		void onRender();

		void onUpdate(Time delta);

		void cleanUp();


	};

}
