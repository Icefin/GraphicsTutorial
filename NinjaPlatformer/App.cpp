#include "App.h"
#include <Gengine/ScreenList.h>

App::App() {

}
App::~App() {

}

void App::onInit() {

}

void App::addScreens() {
	m_gameScreen = std::make_unique<GameScreen>();

	m_screenList->addScreen(m_gameScreen.get());
	m_screenList->setScreen(m_gameScreen->getScreenIndex());
}

void App::onExit() {

}