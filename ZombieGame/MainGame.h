#pragma once

#include <Gengine/Window.h>
#include <Gengine/GLSLProgram.h>
#include <Gengine/Camera2D.h>
#include <Gengine/InputManager.h>
#include "Level.h"

enum class GameState {
	PLAY,
	EXIT
};

class MainGame
{
public :
	MainGame();
	~MainGame();

	void run();

private :
	void initSystem();
	void initShader();
	void initLevel();
	void gameLoop();
	void processInput();
	void drawGame();

	Gengine::Window _window;
	Gengine::GLSLProgram _textureProgram;
	Gengine::InputManager _inputManager;
	Gengine::Camera2D _camera;
	std::vector<Level*> _levels;

	int _screenWidth, _screenHeight;

	GameState _gameState;
	int _fps;
	int _currentLevel;
};