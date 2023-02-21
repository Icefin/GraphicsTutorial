#include "MainGame.h"
#include <Gengine/Gengine.h>
#include <Gengine/Timing.h>

#include <SDL/SDL.h>
#include <iostream>

MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::PLAY),
	_fps(0) {
	//Empty
}

MainGame::~MainGame() {
	for (int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}
}

void MainGame::run() {
	initSystem();
	initLevel();
	gameLoop();
}

void MainGame::initSystem() {
	Gengine::Init();

	_window.Create("ZombieGame", _screenWidth, _screenHeight, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	initShader();

	_camera.Init(_screenWidth, _screenHeight);
}

void MainGame::initLevel() {
	_levels.push_back(new Level("Levels/level1.txt"));
	_currentLevel = 0;
}

void MainGame::initShader() {
	//Compile our color shader
	_textureProgram.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.AddAttribute("vertexPosition");
	_textureProgram.AddAttribute("vertexColor");
	_textureProgram.AddAttribute("vertexUV");
	_textureProgram.LinkShaders();
}

void MainGame::gameLoop() {
	Gengine::FpsLimiter _fpsLimiter;
	_fpsLimiter.SetMaxFPS(60.0f);

	while (_gameState == GameState::PLAY) {
		_fpsLimiter.Begin();
		processInput();
		_camera.Update();
		drawGame();
		_fps = _fpsLimiter.End();
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT :
				//Exit the game
				break;
			case SDL_MOUSEMOTION :
				_inputManager.SetMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_KEYDOWN :
				_inputManager.PressKey(evnt.key.keysym.sym);
			case SDL_KEYUP :
				_inputManager.ReleaseKey(evnt.key.keysym.sym);
			case SDL_MOUSEBUTTONDOWN :
				_inputManager.PressKey(evnt.button.button);
			case SDL_MOUSEBUTTONUP :
				_inputManager.ReleaseKey(evnt.button.button);
				break;
		}
	}
}

void MainGame::drawGame() {
	//Set the base depth to 1.0
	glClearDepth(1.0f);
	//Clear the color and depth  buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.Use();

	//Draw code goes here
	glActiveTexture(GL_TEXTURE0);

	//Make sure the shader uses texture 0
	GLint textureUniform = _textureProgram.GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	//Grab the camera matrix
	glm::mat4 projectionMatrix = _camera.GetCameraMatrix();
	GLint pUniform = _textureProgram.GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Draw the level
	_levels[_currentLevel]->draw();

	_textureProgram.Unuse();

	//Swap our buffer and draw everything to the screen
	_window.SwapBuffer();
}