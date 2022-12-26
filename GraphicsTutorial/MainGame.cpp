#include <iostream>
#include <string>

#include "MainGame.h"
#include "ImageLoader.h"
#include <Errors.h>

MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::PLAY),
	_time(0.0f),
	_maxFPS(60.0f){
	_camera2D.Init(_screenWidth, _screenHeight);
}
MainGame::~MainGame() {

}

void MainGame::Run() {
	InitSystems();

	_sprite.Init(0.0f, 0.0f, _screenWidth / 2, _screenHeight / 2);
	_playerTexture = Gengine::ImageLoader::LoadPNG("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	GameLoop();
}

void MainGame::InitSystems() {
	Gengine::Init();

	_window.Create("Game Engine", _screenWidth, _screenHeight, 0);

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

		_camera2D.Update();

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

	const float CAMERA_SPEED = 20.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT :
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION :
				std::cout << evnt.motion.x << " " << evnt.motion.y << '\n';
				break;
			case SDL_KEYDOWN :
				switch (evnt.key.keysym.sym) {
					case SDLK_w :
						_camera2D.SetPosition(_camera2D.GetPosition() + glm::vec2(0.0f, CAMERA_SPEED));
						break;
					case SDLK_s:
						_camera2D.SetPosition(_camera2D.GetPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
						break;
					case SDLK_a:
						_camera2D.SetPosition(_camera2D.GetPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
						break;
					case SDLK_d:
						_camera2D.SetPosition(_camera2D.GetPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
						break;
					case SDLK_q :
						_camera2D.SetScale(_camera2D.GetScale() + SCALE_SPEED);
						break;
					case SDLK_e :
						_camera2D.SetScale(_camera2D.GetScale() - SCALE_SPEED);
						break;
				}
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

	GLint pLocation = _colorShaderProgram.GetUniformLocation("P");
	glm::mat4 cameraMatrix = _camera2D.GetCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_sprite.Draw();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorShaderProgram.Unuse();

	_window.SwapBuffer();
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