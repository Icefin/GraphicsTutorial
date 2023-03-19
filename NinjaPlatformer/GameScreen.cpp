#include "GameScreen.h"
#include <Gengine/IMainGame.h>
#include <SDL/SDL.h>

GameScreen::GameScreen() {

}

GameScreen::~GameScreen() {

}

void GameScreen::build() {

}

void GameScreen::destroy() {

}
#include <iostream>
void GameScreen::onEntry() {
	std::cout << "OnEntry\n";
}

void GameScreen::onExit() {
}

void GameScreen::update() {
	std::cout << "Update\n";
	checkInput();
}

void GameScreen::draw() {
	std::cout << "Draw\n";
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

int GameScreen::getNextScreenIndex() const {

	return SCREEN_IDX_NO_SCREEN;
}

int GameScreen::getPrevScreenIndex() const {

	return SCREEN_IDX_NO_SCREEN;
}

void GameScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}
