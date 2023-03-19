#pragma once
#include "Gengine.h"
#include "Window.h"
#include "InputManager.h"

#include <memory>


namespace Gengine {
	class ScreenList;
	class IGameScreen;

	class IMainGame
	{
	public :
		IMainGame();
		virtual ~IMainGame();

		void run();
		void exitGame();

		virtual void onInit() = 0;
		virtual void addScreens() = 0;
		virtual void onExit() = 0;

		void onSDLEvent(SDL_Event& evnt);

		const float getFps() const { return (m_fps); }

	protected :
		bool init();
		bool initSystems();

		virtual void update();
		virtual void draw();


		std::unique_ptr<ScreenList> m_screenList = nullptr;
		IGameScreen* m_currentScreen = nullptr;
		bool m_isRunning = false;
		float m_fps = 0.0f;
		Window m_window;
		InputManager m_inputManager;
	};
}

