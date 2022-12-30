#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>

#include <GLSLProgram.h>
#include <GLTexture.h>
#include <Sprite.h>
#include <SpriteBatch.h>

#include <Window.h>
#include <Gengine.h>
#include <Timing.h>

#include <Camera2D.h>
#include <InputManager.h>

#include "Bullet.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public :
	MainGame();
	~MainGame();
	void Run();

private :
	void InitSystems();
	void InitShaders();
	void GameLoop();
	void ProcessInput();
	void DrawGame();

	Gengine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	Gengine::GLSLProgram _colorShaderProgram;
	Gengine::Camera2D _camera2D;
	Gengine::InputManager _inputManager;
	Gengine::FpsLimiter _fpsLimiter;

	Gengine::SpriteBatch _spriteBatch;

	std::vector<Bullet> _bullets;

	float _maxFPS;
	float _fps;
	float _time;
};