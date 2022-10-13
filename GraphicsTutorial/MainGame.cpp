#include <iostream>
#include <string>
#include "MainGame.h"
#include "ImageLoader.h"
#include "Errors.h"

MainGame::MainGame() :
	_window(nullptr),
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::PLAY),
	_time(0.0f) {

}
MainGame::~MainGame() {

}

void MainGame::Run() {
	InitSystems();

	_sprite.Init(-1.0f, -1.0f, 2.0f, 2.0f);
	_playerTexture = ImageLoader::LoadPNG("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	GameLoop();

}

void MainGame::InitSystems() {
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	//Create Window
	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
								_screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		FatalError("SDL Window could not be created");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		FatalError("SDL_GLContext could not be created");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		FatalError("Could not initialize glew");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	InitShaders();
}

void MainGame::InitShaders() {
	_colorShaderProgram.CompileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorShaderProgram.AddAttribute("vertexPosition");
	_colorShaderProgram.AddAttribute("vertexColor");
	_colorShaderProgram.AddAttribute("vertexUV");
	_colorShaderProgram.LinkShaders();
}

void MainGame::GameLoop() {
	while (_gameState != GameState::EXIT){
		ProcessInput();
		_time += 0.1f;
		DrawGame();
	}
}
void MainGame::ProcessInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT :
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION :
				std::cout << evnt.motion.x << " " << evnt.motion.y << '\n';
				break;
		}
	}
}

void MainGame::DrawGame() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorShaderProgram.Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	GLint textureLocation = _colorShaderProgram.GetUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLint timeLocation = _colorShaderProgram.GetUniformLocation("time");
	glUniform1f(timeLocation, _time);

	_sprite.Draw();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorShaderProgram.Unuse();

	SDL_GL_SwapWindow(_window);
}
