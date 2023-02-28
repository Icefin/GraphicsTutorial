#include "MainGame.h"
#include "Zombie.h"
#include "Gun.h"

#include <Gengine/Gengine.h>
#include <Gengine/Timing.h>
#include <Gengine/Errors.h>
#include <SDL/SDL.h>

#include <random>
#include <ctime>
#include <iostream>

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 5.0f;

MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::PLAY),
	_fps(0),
	_player(nullptr),
	_numHumansKilled(0),
	_numZombiesKilled(0) {
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

	_agentSpriteBatch.Init();

	_camera.Init(_screenWidth, _screenHeight);
}

void MainGame::initLevel() {
	_levels.push_back(new Level("Levels/level1.txt"));
	_currentLevel = 0;

	_player = new Player();
	_player->init(PLAYER_SPEED, _levels[_currentLevel]->getPlayerStartPos(), &_inputManager, &_camera, &_bullets);

	_humans.push_back(_player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() - 2);

	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++) {
		_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
	}

	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getZombieStartPos();
	for (int i = 0; i < zombiePositions.size(); i++) {
		_zombies.push_back(new Zombie);
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	const float BULLET_SPEED = 20.0f;
	_player->addGun(new Gun("Magnum", 10, 1, 0.01f, 30, BULLET_SPEED));
	_player->addGun(new Gun("Shotgun", 30, 12, 0.5f, 4, BULLET_SPEED));
	_player->addGun(new Gun("MP5", 2, 1, 0.01f, 20, BULLET_SPEED));
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
		checkVictory();
		processInput();
		updateAgents();
		updateBullets();
		_camera.SetPosition(_player->getPosition());
		_camera.Update();
		drawGame();
		_fps = _fpsLimiter.End();
	}
}

void MainGame::updateAgents() {
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies);
	}

	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies);
	}

	for (int i = 0; i < _zombies.size(); i++) {
		for (int j = i + 1; j < _zombies.size(); j++) {
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		for (int j = 1; j < _humans.size(); j++) {
			if (_zombies[i]->collideWithAgent(_humans[j])) {
				_zombies.push_back(new Zombie);
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}

		if (_zombies[i]->collideWithAgent(_player)) {
			Gengine::FatalError("YOU LOSE");
		}
	}

	for (int i = 0; i < _humans.size(); i++) {
		for (int j = i + 1; j < _humans.size(); j++) {
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}
}

void MainGame::updateBullets() {
	//Update and collide with World
	for (int i = 0; i < _bullets.size();) {
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData())) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	bool wasBulletRemoved;
	//Collide With Agents
	for (int i = 0; i < _bullets.size(); i++) {
		wasBulletRemoved = false;
		//Loop through zombies
		for (int j = 0; j < _zombies.size();) {
			if (_bullets[i].collideWithAgent(_zombies[j])) {
				if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_numZombiesKilled++;
				}
				else {
					j++;
				}
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				wasBulletRemoved = true;
				break;
			}
			else {
				j++;
			}
		}
		if (wasBulletRemoved) continue;
		//Loop through humans
		for (int j = 1; j < _humans.size();) {
			if (_bullets[i].collideWithAgent(_humans[j])) {
				if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
					delete _humans[j];
					_humans[j] = _humans.back();
					_humans.pop_back();
					_numHumansKilled++;
				}
				else {
					j++;
				}
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				break;
			}
			else {
				j++;
			}
		}
	}
}

void MainGame::checkVictory() {
	if (_zombies.empty()) {
		std::printf("*** You Win! ***\n You killed %d humans and %d zombies.\n There are %d/%d humans remaining",
			_numHumansKilled, _numZombiesKilled, _humans.size() - 1, _levels[_currentLevel]->getNumHumans());
		Gengine::FatalError("");
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT :
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION :
				_inputManager.SetMouseCoords(evnt.motion.x, evnt.motion.y);
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

	//Draw the humans
	_agentSpriteBatch.Begin();
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->draw(_agentSpriteBatch);
	}

	//Draw the zombies
	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->draw(_agentSpriteBatch);
	}

	//Draw the bullets
	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.End();
	_agentSpriteBatch.RenderBatchs();

	_textureProgram.Unuse();

	//Swap our buffer and draw everything to the screen
	_window.SwapBuffer();
}