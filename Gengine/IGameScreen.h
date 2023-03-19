#pragma once

#define SCREEN_IDX_NO_SCREEN -1

namespace Gengine {
	class IMainGame;

	enum class ScreenState { NONE, RUNNING, CHANGE_NEXT, CHANGE_PREV, EXIT_APPLICATION };

	class IGameScreen
	{
	public :
		friend class ScreenList;

		IGameScreen() {};
		virtual ~IGameScreen() {};

		//Called at beginning and end of application
		virtual void build() = 0;
		virtual void destroy() = 0;

		//Called when a screen enters and exits focus
		virtual void onEntry() = 0;
		virtual void onExit() = 0;

		//Called in the main game loop
		virtual void update() = 0;
		virtual void draw() = 0;

		int getScreenIndex() const { return (m_screenIdx); }
		virtual int getNextScreenIndex() const = 0;
		virtual int getPrevScreenIndex() const = 0;

		ScreenState getState() const { return (m_currentState); }

		void setParentGame(IMainGame* game) { m_game = game; }
		void setRunning() { m_currentState = ScreenState::RUNNING; }

	protected :
		ScreenState m_currentState = ScreenState::NONE;
		IMainGame* m_game = nullptr;
		int m_screenIdx = -1;
	};
}