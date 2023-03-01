#include <iostream>
#include <string>

#include "MainGame.h"
#include <Errors.h>
#include <ResourceManager.h>

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
	GameLoop();
}

void MainGame::InitSystems() {
	Gengine::Init();

	_window.Create("Game Engine", _screenWidth, _screenHeight, 0);

	InitShaders();

	_spriteBatch.Init();
	_fpsLimiter.Init(_maxFPS);
}

void MainGame::InitShaders() {
	_colorShaderProgram.CompileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorShaderProgram.AddAttribute("vertexPosition");
	_colorShaderProgram.AddAttribute("vertexColorRGBA8");
	_colorShaderProgram.AddAttribute("vertexUV");
	_colorShaderProgram.LinkShaders();
}

void MainGame::GameLoop() {
	while (_gameState != GameState::EXIT){
		//Used for frame time measuring
		_fpsLimiter.Begin();

		ProcessInput();
		_time += 0.01f;

		_camera2D.Update();

		for (int i = 0; i < _bullets.size();) {
			if (_bullets[i].Update()) {
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else {
				i++;
			}
		}

		DrawGame();

		_fps = _fpsLimiter.End();

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10000) {
			std::cout << "FPS : " << _fps << '\n';
			frameCounter = 0;
		}
	}
}
void MainGame::ProcessInput() {
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT :
				_gameState = GameState::EXIT;
				break;
			case SDL_KEYDOWN :
				_inputManager.PressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP :
				_inputManager.ReleaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN :
				_inputManager.PressKey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				_inputManager.ReleaseKey(evnt.button.button);
				break;
			case SDL_MOUSEMOTION:
				_inputManager.SetMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
		}
	}

	if (_inputManager.isKeyPressed(SDLK_w)) {
		_camera2D.SetPosition(_camera2D.GetPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_s)) {
		_camera2D.SetPosition(_camera2D.GetPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_a)) {
		_camera2D.SetPosition(_camera2D.GetPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_d)) {
		_camera2D.SetPosition(_camera2D.GetPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_q)) {
		_camera2D.SetScale(_camera2D.GetScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_e)) {
		_camera2D.SetScale(_camera2D.GetScale() - SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.GetMouseCoords();
		mouseCoords = _camera2D.ConvertScreenToWorld(mouseCoords);
		
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		_bullets.emplace_back(playerPosition, direction, 1.0f, 1000);
	}
}

void MainGame::DrawGame() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorShaderProgram.Use();

	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _colorShaderProgram.GetUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLint pLocation = _colorShaderProgram.GetUniformLocation("P");
	glm::mat4 cameraMatrix = _camera2D.GetCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.Begin();
	
	glm::vec4 position(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Gengine::GLTexture texture = Gengine::ResourceManager::GetTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	Gengine::ColorRGBA8 color;
	color.r = 255; color.g = 255; color.b = 255; color.a = 255;
	_spriteBatch.Draw(position, uv, texture.id, 0.0f, color);

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].Draw(_spriteBatch);
	}

	_spriteBatch.End();
	
	_spriteBatch.RenderBatchs();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorShaderProgram.Unuse();

	_window.SwapBuffer();
}