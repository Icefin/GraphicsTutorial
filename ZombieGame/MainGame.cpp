#include "MainGame.h"
#include "../deps/include/SDL/SDL.h"

MainGame::MainGame() {
	//Empty
}

MainGame::~MainGame() {
	//Empty
}

void MainGame::run() {
	//Empty
}

void MainGame::initSystem() {
	//Empty
}

void MainGame::initShader() {
	//Compile our color shader
	_textureProgram.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.AddAttribute("vertexPosition");
	_textureProgram.AddAttribute("vertexColor");
	_textureProgram.AddAttribute("vertexUV");
}

void MainGame::gameLoop() {
	//Empty
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

	//Swap our buffer and draw everything to the screen
	_window.SwapBuffer();
}