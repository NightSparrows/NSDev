
#include <NS/core/Log.h>
#include <NS/core/GameEngine.h>
#include <NS/core/io/Clock.h>
#include <NS/core/graphics/MasterRenderer.h>
#include <NS/core/audio/MasterAudio.h>
#include <NS/core/parallelComputing/MasterCL.h>

namespace ns {

	GameEngine *GameEngine::s_GameEngine = nullptr;

	// Show the engine that take the proformance
	template<typename Fn>
	class EngineProfile {
	public:
		EngineProfile(const char *name, Fn&& func)
			: m_Name(name), m_func(func), m_stopped(false) 
		{
			this->m_startTimepoint = Time::GetTime();
		}
		~EngineProfile() {
			if (!m_stopped)
				stop();
		}

		void stop() {
			float endTimepoint = Time::GetTime();

			float duration = endTimepoint - this->m_startTimepoint;

			this->m_func({ this->m_Name, duration });
		}

	private:
		const char *m_Name;
		float m_startTimepoint;
		bool m_stopped;
		Fn m_func;
	};
#define PROFILE_SCOPE(name)	EngineProfile timer##__LINE__(name, [&](ProfileResult profileResult) {this->m_profileResults.push_back(profileResult); });
	// End engine profile

	void GameEngine::run(GameState *startState) {
		if (this->m_Running) {
			NS_CORE_ASSERT(false, "Engine is already running!");
			return;
		}

		// this->m_Window->create(style);

		this->m_Running = true;

		// Starting, Initialize some important Masters
		MasterCL::Init();
		MasterAudio::Init();
		// ResourceManager::Init();
		MasterRenderer::Init();

		this->m_StateManager->init(startState);
		// End initialization


		ns::Clock clock;

		float counter = 0;
		int calcFps = 0;

		while (this->m_Running) {
			PROFILE_SCOPE("Engine total frametime")
			Time time = clock.restart();

			// update
			{
				PROFILE_SCOPE("Engine Update")
				this->m_Window->update();
				this->m_StateManager->onUpdate(time);
				MasterRenderer::Update(time);
			}

			counter += time;
			{
				PROFILE_SCOPE("Engine Render")
				// Render this frame
				MasterRenderer::BeginFrame();
				this->m_StateManager->onRender();
				MasterRenderer::EndFrame();
			}
			this->m_Window->display();
			calcFps++;

			if (counter >= 1.0f) {

				this->m_CurrentFps = calcFps;
				
				calcFps = 0;
				counter = 0;
			}

			// clear the profile
			/*
			for (auto& result : this->m_profileResults) {
				char label[50];
				sprintf(label, "%s %.3fms", result.name, (result.time * 1000.f));
				NS_CORE_INFO(label);
			}
			*/
			this->m_profileResults.clear();
		}

		// clean up
		this->m_StateManager->cleanUp();

		MasterRenderer::CleanUp();
		// ResourceManager::CleanUp();
		//MasterAudio::CleanUp();
		MasterCL::CleanUp();

	}

	void GameEngine::Create(const WindowProperties& properties) {
		
		// Initialize the logger
		Log::Init();

		if (s_GameEngine != nullptr) {
			Destroy();
		}
		s_GameEngine = new GameEngine(properties);
	}

	void GameEngine::Run(GameState *startState) {
		try {
			s_GameEngine->run(startState);
		}
		catch (const std::exception &e) {
#ifdef NS_DEBUG
			NS_CORE_ERROR("Exception:{0}", e.what());
#endif
		}
	}

	void GameEngine::Stop() {
		s_GameEngine->m_Running = false;
	}

	void GameEngine::AddState(GameState *gameState) {
		s_GameEngine->m_StateManager->addState(gameState);
	}

	void GameEngine::ChangeState(const char *stateName) {
		s_GameEngine->m_StateManager->changeState(stateName);
	}

	// callback
	void GameEngine::WindowCallback(ns::Event &event) {
		this->m_StateManager->onEvent(event);
	}

	GameEngine::GameEngine(const WindowProperties& properties) {
		this->m_Running = false;
		this->m_Window = Window::Create(properties);
		this->m_StateManager = CreateScope<GameStateManager>();
		this->m_Window->setEventCallback(NS_BIND_EVENT_FN(GameEngine::WindowCallback));
#ifdef NS_DEBUG
		NS_CORE_INFO("Sparrow Engine created.");
#endif

	}

	GameEngine::~GameEngine() {
#ifdef NS_DEBUG
		NS_CORE_INFO("Sparrow Engine stopped.");
#endif
	}

	void GameEngine::Destroy() {

		delete s_GameEngine;
	}

	void GameEngine::SetFPS(int fps) {
		s_GameEngine->m_Fps = fps;
		s_GameEngine->m_Spf = 1.0f / (float)fps;
	}

	bool GameEngine::IsRunning() {
		return s_GameEngine->m_Running;
	}
}
