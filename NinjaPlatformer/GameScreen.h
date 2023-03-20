#pragma once
#include "Box.h"
#include <Box2D/Box2D.h>
#include <Gengine/IGameScreen.h>
#include <Gengine/SpriteBatch.h>
#include <Gengine/GLSLProgram.h>
#include <Gengine/Camera2D.h>
#include <Gengine/GLTexture.h>
#include <Gengine/Window.h>
#include <vector>

class GameScreen : public Gengine::IGameScreen {
public :
	GameScreen(Gengine::Window* window);
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

	Gengine::GLSLProgram m_textureProgram;
	Gengine::Camera2D m_camera;
	Gengine::SpriteBatch m_spriteBatch;
	Gengine::GLTexture m_texture;
	Gengine::Window* m_window;

	std::unique_ptr<b2World> m_world;
	std::vector<Box> m_boxes;
};

