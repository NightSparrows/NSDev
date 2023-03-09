#pragma once

#include <NS/SparrowEngine.h>


// For Computer graphic final project
class HWState : public ns::GameState {
public:
	HWState() : GameState("hw") {

	}

	void init() override;

	void onUpdate(ns::Time delta) override;

	void onEvent(ns::Event &event) override;

	void onRender() override;

	void cleanUp() override;

};
