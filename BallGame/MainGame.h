#pragma once
#include <Gengine/Camera2D.h>
#include <Gengine/SpriteBatch.h>
#include <Gengine/InputManager.h>
#include <Gengine/Window.h>
#include <Gengine/GLSLProgram.h>
#include <Gengine/Timing.h>
#include <Gengine/SpriteFont.h>

#include <memory>

#include "BallController.h"
#include "BallRenderer.h"
#include "Grid.h"

enum class GameState { RUNNING, EXIT };

const int CELL_SIZE = 12;

class MainGame
{
public :
	~MainGame();
	void run();

private :
	void init();
	void initRenderers();
	void initBalls();
	void update(float deltaTime);
	void draw();
	void drawUI();
	void processInput();

	int m_screenWidth = 0;
	int m_screenHeight = 0;

	std::vector<Ball> m_balls;
	std::unique_ptr<Grid> m_grid;

	int m_currentRendererIdx = 0;
	std::vector<std::unique_ptr<BallRenderer>> m_ballRenderers;

	BallController m_ballController;

	Gengine::Window m_window;
	Gengine::SpriteBatch m_spriteBatch;
	std::unique_ptr<Gengine::SpriteFont> m_spriteFont;
	Gengine::Camera2D m_camera;
	Gengine::InputManager m_inputManager;
	Gengine::GLSLProgram m_textureProgram;

	Gengine::FpsLimiter m_fpsLimiter;
	float m_fps = 0.0f;

	GameState m_gameState = GameState::RUNNING;
};

