#pragma once

#include "io/Window.h"
#include "io/EngineProfile.h"
#include "GameStateManager.h"

namespace ns {

	/**
		For a application to run this GameEngine(framework?)

	*/
	class NS_API GameEngine {
	public:
		/// Initialize the logger
		/// Setup Engine with a window.
		/// You must use this method to before other function to call
		/// Otherwise it will crash.
		static void Create(const WindowProperties& properties = WindowProperties());

		/// Start the Engine
		/// A state to start
		static void Run(GameState *startState); 

		/// Stop the Engine
		static void Stop();

		/// Destroy the engine
		/// Destroy the engine if you want to create a new engine
		static void Destroy();

		/// Get the current Frame per second (How much does the engine render.)
		static int GetFps() { return s_GameEngine->m_CurrentFps; }

		
		/// Set the Maxinum FPS that Engine can run
		static void SetFPS(int fps);

		// The current window class for usage
		static const Window *GetWindow() { return s_GameEngine->m_Window.get(); }

		// Add a game state to the engine, not run the state just initialize
		static void AddState(GameState *gameState);

		// Add a game state to the engine, not run the state just initialize
		static void ChangeState(const char* stateName);

		// Whether the engine is running
		static bool IsRunning();

	private:
		GameEngine(const WindowProperties& properties);
		~GameEngine();

		void run(GameState *startState);

		// Setting for window callback function
		void WindowCallback(ns::Event &event);

	private:
		bool m_Running;
		Scope<Window> m_Window;
		Scope<GameStateManager> m_StateManager;

		int m_Fps = 120;
		float m_Spf = 1.0f / (float)m_Fps;
		int m_CurrentFps = 0;

		// TODO 差看各個所花的時間
		std::vector<ProfileResult> m_profileResults;

		// static
		static GameEngine *s_GameEngine;

	};

}

