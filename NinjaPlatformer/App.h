#pragma once
#include <Gengine/IMainGame.h>
#include "GameScreen.h"

class App : public Gengine::IMainGame {
public :
	App();
	~App();

	virtual void onInit() override;
	virtual void addScreens() override;
	virtual void onExit() override;

private :
	std::unique_ptr<GameScreen> m_gameScreen = nullptr;
};

