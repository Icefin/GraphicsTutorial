#pragma once
#include "Level.h"
#include "Player.h"

#include <Gengine/Window.h>
#include <Gengine/GLSLProgram.h>
#include <Gengine/Camera2D.h>
#include <Gengine/InputManager.h>
#include <Gengine/SpriteBatch.h>

class Zombie;

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
	void updateAgents();
	void processInput();
	void drawGame();

	Gengine::Window _window;
	Gengine::GLSLProgram _textureProgram;
	Gengine::InputManager _inputManager;
	Gengine::Camera2D _camera;
	Gengine::SpriteBatch _agentSpriteBatch;
	std::vector<Level*> _levels;

	int _screenWidth, _screenHeight;

	GameState _gameState;
	float _fps;
	int _currentLevel;

	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;
	Player* _player;
};