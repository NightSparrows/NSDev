
#include <NS/SparrowEngine.h>
#include "StartState.h"

// for computer graphic final project
#include "FinalProjectState.h"

int main(int argc, char** argv) {

	ns::GameEngine::Create(ns::WindowProperties("Sandbox", 1280, 720));

	StartState state;

	// for computer graphic final project
	HWState hwState;


	ns::GameEngine::Run(&hwState);

	ns::GameEngine::Destroy();

	return EXIT_SUCCESS;
}
