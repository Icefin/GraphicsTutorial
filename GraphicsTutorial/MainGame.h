#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>

#include <GLSLProgram.h>
#include <GLTexture.h>
#include <Sprite.h>
#include <SpriteBatch.h>

#include <Window.h>
#include <Gengine.h>

#include <Camera2D.h>

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
	void CalculateFPS();

	Gengine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;


	Gengine::GLSLProgram _colorShaderProgram;
	Gengine::Camera2D _camera2D;

	Gengine::SpriteBatch _spriteBatch;

	float _fps;
	float _maxFPS;
	float _frameTime;

	float _time;
};