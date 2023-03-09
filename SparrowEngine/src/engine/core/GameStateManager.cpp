//
// Created by ªô¤l·¶ on 2019/12/14.
//


#include <NS/core/Log.h>

#include <NS/core/GameStateManager.h>

#include <NS/core/io/Event.h>

namespace ns {

	GameStateManager::GameStateManager() {
		this->shouldChangeState = false;
	}

	void GameStateManager::init(GameState *startState) {
		this->currentState = startState;

		this->currentState->init();
		this->states.push_back(this->currentState);
	}

	void GameStateManager::addState(GameState *state) {
		state->init();
		this->states.push_back(state);
	}

	void GameStateManager::changeState(const char *stateName) {
		for (GameState *state : this->states) {
			if (state->getName().compare(stateName) == 0) {
				this->nextState = state;
				this->shouldChangeState = true;
				return;
			}
		}
		NS_CORE_ERROR("State not found:{0}", stateName);
	}

	void GameStateManager::onEvent(Event& event) {
		this->currentState->onEvent(event);
	}

	void GameStateManager::onUpdate(Time delta) {

		// update
		this->currentState->onUpdate(delta);

		// If shouldChangeState is true will change state
		if (this->shouldChangeState) {
			NS_CORE_ASSERT((this->nextState != nullptr), "Change state ASSERT\nNext state is null pointer!");
			this->currentState->onPause();
			this->currentState = this->nextState;
			this->currentState->onChange();
			this->nextState = nullptr;
			this->shouldChangeState = false;
		}
	}

	void GameStateManager::onRender() {
		this->currentState->onRender();
	}

	void GameStateManager::cleanUp() {
		for (GameState *state : this->states) {
			state->cleanUp();
		}
		this->states.clear();
	}
}