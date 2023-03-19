#pragma once
#include <Gengine/IGameScreen.h>

class GameScreen : public Gengine::IGameScreen {
public :
	GameScreen();
	~GameScreen();

	virtual void build() override;
	virtual void destroy() override;
	virtual void onEntry() override;
	virtual void onExit() override;
	virtual void update() override;
	virtual void draw() override;
	virtual int getNextScreenIndex() const override;
	virtual int getPrevScreenIndex() const override;

private :
	void checkInput();
};

