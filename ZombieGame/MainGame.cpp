#include "MainGame.h"
#include "Zombie.h"
#include "Gun.h"

#include <Gengine/Gengine.h>
#include <Gengine/Timing.h>
#include <Gengine/Errors.h>
#include <Gengine/ResourceManager.h>
#include <SDL/SDL.h>
#include <glm/gtx/rotate_vector.hpp>

#include <random>
#include <ctime>
#include <iostream>
#include <algorithm>

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

	for (int i = 0; i < _humans.size(); i++) {
		delete _humans[i];
	}

	for (int i = 0; i < _zombies.size(); i++) {
		delete _zombies[i];
	}
}

void MainGame::run() {
	initSystem();
	initLevel();

	Gengine::Music bgm = _audioEngine.loadMusic("Sound/XYZ.ogg");
	bgm.play();

	gameLoop();
}

void MainGame::initSystem() {
	Gengine::Init();
	
	_audioEngine.init();

	_window.Create("ZombieGame", _screenWidth, _screenHeight, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	initShader();

	_agentSpriteBatch.Init();
	_uiSpriteBatch.Init();

	_spriteFont = new Gengine::SpriteFont("Fonts/chintzy.ttf", 32);

	_camera.Init(_screenWidth, _screenHeight);
	_uiCamera.Init(_screenWidth, _screenHeight);
	_uiCamera.SetPosition(glm::vec2(_screenWidth / 2, _screenHeight / 2));

	_bloodParticleBatch = new Gengine::ParticleBatch2D;
	_bloodParticleBatch->init(1000, 0.05f, Gengine::ResourceManager::GetTexture("Textures/particle.png"),
		[](Gengine::Particle2D& particle, float deltaTime) {
			particle.position += particle.velocity * deltaTime;
			particle.color.a = (GLubyte)(particle.life * 255.0f);
		});
	_particleEngine2D.addParticleBatch(_bloodParticleBatch);
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
	_player->addGun(new Gun("Magnum", 10, 1, 0.01f, 30, BULLET_SPEED, _audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
	_player->addGun(new Gun("Shotgun", 30, 12, 0.5f, 4, BULLET_SPEED, _audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
	_player->addGun(new Gun("MP5", 2, 1, 0.1f, 20, BULLET_SPEED, _audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));
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

	const float CAMERA_SCALE = 1.0f / 4.0f;
	_camera.SetScale(CAMERA_SCALE);

	const int MAX_PHYSICS_STEPS = 6;
	const float DESIRED_FPS = 60;
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	float prevTicks = SDL_GetTicks();

	while (_gameState == GameState::PLAY) {
		_fpsLimiter.Begin();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		checkVictory();
		processInput();

		int step_cnt = 0;
		while (totalDeltaTime > 0.0f && step_cnt < MAX_PHYSICS_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			_particleEngine2D.update(deltaTime);
			totalDeltaTime -= deltaTime;
			step_cnt++;
		}

		_camera.SetPosition(_player->getPosition());
		_camera.Update();
		_uiCamera.Update();
		drawGame();
		_fps = _fpsLimiter.End();
	}
}

void MainGame::updateAgents(float deltaTime) {
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}

	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
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

void MainGame::updateBullets(float deltaTime) {
	//Update and collide with World
	for (int i = 0; i < _bullets.size();) {
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime)) {
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
				addBlood(_bullets[i].getPosition(), 5);
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
				addBlood(_bullets[i].getPosition(), 5);
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

	const glm::vec2 agentDim(AGENT_RADIUS * 2);

	for (int i = 0; i < _humans.size(); i++) {
		if (_camera.isBoxInView(_humans[i]->getPosition(), agentDim))
			_humans[i]->draw(_agentSpriteBatch);
	}

	//Draw the zombies
	for (int i = 0; i < _zombies.size(); i++) {
		if (_camera.isBoxInView(_zombies[i]->getPosition(), agentDim))
			_zombies[i]->draw(_agentSpriteBatch);
	}

	//Draw the bullets
	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.End();
	_agentSpriteBatch.RenderBatchs();

	_particleEngine2D.draw(&_agentSpriteBatch);

	drawUI();

	_textureProgram.Unuse();

	//Swap our buffer and draw everything to the screen
	_window.SwapBuffer();
}

void MainGame::drawUI() {
	char buffer[256];

	glm::mat4 projectionMatrix = _uiCamera.GetCameraMatrix();
	GLint pUniform = _textureProgram.GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_uiSpriteBatch.Begin();

	sprintf_s(buffer, "Num Humans %d", _humans.size());
	_spriteFont->draw(_uiSpriteBatch, buffer, glm::vec2(130, 0),
						glm::vec2(1.0), 0.0f, Gengine::ColorRGBA8(255, 255, 255, 255));

	sprintf_s(buffer, "Num Zombies %d", _zombies.size());
	_spriteFont->draw(_uiSpriteBatch, buffer, glm::vec2(130, 40),
		glm::vec2(1.0), 0.0f, Gengine::ColorRGBA8(255, 255, 255, 255));

	_uiSpriteBatch.End();
	_uiSpriteBatch.RenderBatchs();
}

void MainGame::addBlood(const glm::vec2& position, int numParticles) {
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);
	
	glm::vec2 velocity(2.0f, 0.0f);

	Gengine::ColorRGBA8 color(255, 0, 0, 255);

	for (int i = 0; i < numParticles; i++) {
		_bloodParticleBatch->addParticle(position, glm::rotate(velocity, randAngle(randEngine)), color, 20.0f);
	}
}