#pragma once
#include <vector>

namespace Gengine {
	class IMainGame;
	class IGameScreen;

	class ScreenList
	{
	public :
		ScreenList(IMainGame* game);
		~ScreenList();

		IGameScreen* moveNext();
		IGameScreen* movePrev();

		void setScreen(int nextScreen);
		void addScreen(IGameScreen* newScreen);

		void destroy();

		IGameScreen* getCurrent();

	protected :
		std::vector<IGameScreen*> m_screens;
		IMainGame* m_game = nullptr;
		int m_currentScreenIdx = -1;
	};
}

