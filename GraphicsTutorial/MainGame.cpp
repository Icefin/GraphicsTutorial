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
	_time(0.0f),
	_maxFPS(60.0f){

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

	//Tell SDL that we want a double buffered window so we don't get any flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	//Open an SDL window
	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
								_screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		FatalError("SDL Window could not be created");
	}

	//Set up OpenGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		FatalError("SDL_GLContext could not be created");
	}

	//Set up glew
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		FatalError("Could not initialize glew");
	}

	//Check the OpenGL version
	std::printf("***	OpenGL Version : %s		***", glGetString(GL_VERSION));

	//Set the background color to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Set VSYNC
	SDL_GL_SetSwapInterval(0);

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
		//Used for frame time measuring
		float startTicks = SDL_GetTicks();

		ProcessInput();
		_time += 0.01f;
		DrawGame();
		CalculateFPS();

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10) {
			std::cout << _fps << '\n';
			frameCounter = 0;
		}

		float frameTicks = SDL_GetTicks() - startTicks;

		//Limit the fps to the max FPS
		if (1000.0f / _maxFPS > frameTicks) {
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		}
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

void MainGame::CalculateFPS() {
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currFrame = 0;

	static float prevTicks = SDL_GetTicks();

	float currTicks = SDL_GetTicks();

	_frameTime = currTicks - prevTicks;
	frameTimes[currFrame % NUM_SAMPLES] = _frameTime;

	prevTicks = currTicks;

	int cnt;

	currFrame++;
	if (currFrame < NUM_SAMPLES) {
		cnt = currFrame;
	}
	else {
		cnt = NUM_SAMPLES;
	}

	float frameTimeAverage = 0;
	for (int i = 0; i < cnt; i++) {
		frameTimeAverage += frameTimes[i];
	}

	frameTimeAverage /= cnt;

	if (frameTimeAverage > 0) {
		_fps = 1000.0f / frameTimeAverage;
	}
	else {
		_fps = 60.0f;
	}
}