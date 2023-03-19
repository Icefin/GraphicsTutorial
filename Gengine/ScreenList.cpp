#include "ScreenList.h"
#include "IGameScreen.h"

namespace Gengine {
	ScreenList::ScreenList(IMainGame* game) :
	m_game(game) {

	}

	ScreenList::~ScreenList() {
		destroy();
	}

	IGameScreen* ScreenList::moveNext() {
		IGameScreen* currentScreen = getCurrent();
		if (currentScreen->getNextScreenIndex() != SCREEN_IDX_NO_SCREEN) {
			m_currentScreenIdx = currentScreen->getNextScreenIndex();
		}
		return getCurrent();
	}

	IGameScreen* ScreenList::movePrev() {
		IGameScreen* currentScreen = getCurrent();
		if (currentScreen->getPrevScreenIndex() != SCREEN_IDX_NO_SCREEN) {
			m_currentScreenIdx = currentScreen->getPrevScreenIndex();
		}
		return getCurrent();
	}

	void ScreenList::setScreen(int nextScreen) {
		m_currentScreenIdx = nextScreen;
	}

	void ScreenList::addScreen(IGameScreen* newScreen) {
		m_screens.push_back(newScreen);
		newScreen->build();
		newScreen->setParentGame(m_game);
	}

	void ScreenList::destroy() {
		for (auto& screen : m_screens) {
			screen->destroy();
			delete screen;
		}
		m_screens.resize(0);
		m_currentScreenIdx = SCREEN_IDX_NO_SCREEN;
	}

	IGameScreen* ScreenList::getCurrent() {
		if (m_currentScreenIdx == SCREEN_IDX_NO_SCREEN) return (nullptr);
		return (m_screens[m_currentScreenIdx]);
	}
}
