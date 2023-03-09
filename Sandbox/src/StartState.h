#pragma once

#include <NS/SparrowEngine.h>

class StartState : public ns::GameState {
public:
	StartState() : GameState("start") {

	}

	void init() override;

	void onUpdate(ns::Time delta) override;

	void onEvent(ns::Event &event) override;

	void onRender() override;

	void cleanUp() override;

};
