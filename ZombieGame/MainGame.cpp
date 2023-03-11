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
	m_screenWidth(1024),
	m_screenHeight(768),
	m_gameState(GameState::PLAY),
	m_fps(0),
	m_player(nullptr),
	m_numHumansKilled(0),
	m_numZombiesKilled(0) {
	//Empty
}

MainGame::~MainGame() {
	for (int i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}

	for (int i = 0; i < m_humans.size(); i++) {
		delete m_humans[i];
	}

	for (int i = 0; i < m_zombies.size(); i++) {
		delete m_zombies[i];
	}
}

void MainGame::run() {
	initSystem();
	initLevel();

	Gengine::Music bgm = m_audioEngine.loadMusic("Sound/XYZ.ogg");
	bgm.play();

	gameLoop();
}

void MainGame::initSystem() {
	Gengine::Init();
	
	m_audioEngine.init();

	m_window.Create("ZombieGame", m_screenWidth, m_screenHeight, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	initShader();

	m_agentSpriteBatch.Init();
	m_uiSpriteBatch.Init();

	m_spriteFont = new Gengine::SpriteFont("Fonts/chintzy.ttf", 32);

	m_camera.Init(m_screenWidth, m_screenHeight);
	m_uiCamera.Init(m_screenWidth, m_screenHeight);
	m_uiCamera.SetPosition(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));

	m_bloodParticleBatch = new Gengine::ParticleBatch2D;
	m_bloodParticleBatch->init(1000, 0.05f, Gengine::ResourceManager::GetTexture("Textures/particle.png"),
		[](Gengine::Particle2D& particle, float deltaTime) {
			particle.position += particle.velocity * deltaTime;
			particle.color.a = (GLubyte)(particle.life * 255.0f);
		});
	m_particleEngine2D.addParticleBatch(m_bloodParticleBatch);
}

void MainGame::initLevel() {
	m_levels.push_back(new Level("Levels/level1.txt"));
	m_currentLevel = 0;

	m_player = new Player();
	m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getPlayerStartPos(), &m_inputManager, &m_camera, &m_bullets);

	m_humans.push_back(m_player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(2, m_levels[m_currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> randY(2, m_levels[m_currentLevel]->getHeight() - 2);

	for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++) {
		m_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		m_humans.back()->init(HUMAN_SPEED, pos);
	}

	const std::vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getZombieStartPos();
	for (int i = 0; i < zombiePositions.size(); i++) {
		m_zombies.push_back(new Zombie);
		m_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	const float BULLET_SPEED = 20.0f;
	m_player->addGun(new Gun("Magnum", 10, 1, 0.01f, 30, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
	m_player->addGun(new Gun("Shotgun", 30, 12, 0.5f, 4, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
	m_player->addGun(new Gun("MP5", 2, 1, 0.1f, 20, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));
}

void MainGame::initShader() {
	//Compile our color shader
	m_textureProgram.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.AddAttribute("vertexPosition");
	m_textureProgram.AddAttribute("vertexColor");
	m_textureProgram.AddAttribute("vertexUV");
	m_textureProgram.LinkShaders();
}

void MainGame::gameLoop() {
	Gengine::FpsLimiter fpsLimiter;
	fpsLimiter.SetMaxFPS(60.0f);

	const float CAMERA_SCALE = 1.0f / 4.0f;
	m_camera.SetScale(CAMERA_SCALE);

	const int MAX_PHYSICS_STEPS = 6;
	const float DESIRED_FPS = 60;
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	float prevTicks = SDL_GetTicks();

	while (m_gameState == GameState::PLAY) {
		fpsLimiter.Begin();

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
			m_particleEngine2D.update(deltaTime);
			totalDeltaTime -= deltaTime;
			step_cnt++;
		}

		m_camera.SetPosition(m_player->getPosition());
		m_camera.Update();
		m_uiCamera.Update();
		drawGame();
		m_fps = fpsLimiter.End();
	}
}

void MainGame::updateAgents(float deltaTime) {
	for (int i = 0; i < m_humans.size(); i++) {
		m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);
	}

	for (int i = 0; i < m_zombies.size(); i++) {
		m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);
	}

	for (int i = 0; i < m_zombies.size(); i++) {
		for (int j = i + 1; j < m_zombies.size(); j++) {
			m_zombies[i]->collideWithAgent(m_zombies[j]);
		}
		for (int j = 1; j < m_humans.size(); j++) {
			if (m_zombies[i]->collideWithAgent(m_humans[j])) {
				m_zombies.push_back(new Zombie);
				m_zombies.back()->init(ZOMBIE_SPEED, m_humans[j]->getPosition());
				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}
		}

		if (m_zombies[i]->collideWithAgent(m_player)) {
			Gengine::FatalError("YOU LOSE");
		}
	}

	for (int i = 0; i < m_humans.size(); i++) {
		for (int j = i + 1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}
}

void MainGame::updateBullets(float deltaTime) {
	//Update and collide with World
	for (int i = 0; i < m_bullets.size();) {
		if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime)) {
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	bool wasBulletRemoved;
	//Collide With Agents
	for (int i = 0; i < m_bullets.size(); i++) {
		wasBulletRemoved = false;
		//Loop through zombies
		for (int j = 0; j < m_zombies.size();) {
			if (m_bullets[i].collideWithAgent(m_zombies[j])) {
				addBlood(m_bullets[i].getPosition(), 5);
				if (m_zombies[j]->applyDamage(m_bullets[i].getDamage())) {
					delete m_zombies[j];
					m_zombies[j] = m_zombies.back();
					m_zombies.pop_back();
					m_numZombiesKilled++;
				}
				else {
					j++;
				}
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				wasBulletRemoved = true;
				break;
			}
			else {
				j++;
			}
		}
		if (wasBulletRemoved) continue;
		//Loop through humans
		for (int j = 1; j < m_humans.size();) {
			if (m_bullets[i].collideWithAgent(m_humans[j])) {
				addBlood(m_bullets[i].getPosition(), 5);
				if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) {
					delete m_humans[j];
					m_humans[j] = m_humans.back();
					m_humans.pop_back();
					m_numHumansKilled++;
				}
				else {
					j++;
				}
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				break;
			}
			else {
				j++;
			}
		}
	}
}

void MainGame::checkVictory() {
	if (m_zombies.empty()) {
		std::printf("*** You Win! ***\n You killed %d humans and %d zombies.\n There are %d/%d humans remaining",
			m_numHumansKilled, m_numZombiesKilled, m_humans.size() - 1, m_levels[m_currentLevel]->getNumHumans());
		Gengine::FatalError("");
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT :
				m_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION :
				m_inputManager.SetMouseCoords(evnt.motion.x - 125, evnt.motion.y);
				break;
			case SDL_KEYDOWN :
				m_inputManager.PressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP :
				m_inputManager.ReleaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN :
				m_inputManager.PressKey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP :
				m_inputManager.ReleaseKey(evnt.button.button);
				break;
		}
	}
}

void MainGame::drawGame() {
	//Set the base depth to 1.0
	glClearDepth(1.0f);
	//Clear the color and depth  buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_textureProgram.Use();

	//Draw code goes here
	glActiveTexture(GL_TEXTURE0);

	//Make sure the shader uses texture 0
	GLint textureUniform = m_textureProgram.GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	//Grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.GetCameraMatrix();
	GLint pUniform = m_textureProgram.GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Draw the level
	m_levels[m_currentLevel]->draw();

	//Draw the humans
	m_agentSpriteBatch.Begin();

	const glm::vec2 agentDim(AGENT_RADIUS * 2.0f);

	for (int i = 0; i < m_humans.size(); i++) {
		if (m_camera.isBoxInView(m_humans[i]->getPosition(), agentDim))
			m_humans[i]->draw(m_agentSpriteBatch);
	}

	//Draw the zombies
	for (int i = 0; i < m_zombies.size(); i++) {
		if (m_camera.isBoxInView(m_zombies[i]->getPosition(), agentDim))
			m_zombies[i]->draw(m_agentSpriteBatch);
	}

	//Draw the bullets
	for (int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i].draw(m_agentSpriteBatch);
	}

	m_agentSpriteBatch.End();
	m_agentSpriteBatch.RenderBatchs();

	m_particleEngine2D.draw(&m_agentSpriteBatch);

	drawUI();

	m_textureProgram.Unuse();

	//Swap our buffer and draw everything to the screen
	m_window.SwapBuffer();
}

void MainGame::drawUI() {
	char buffer[256];

	glm::mat4 projectionMatrix = m_uiCamera.GetCameraMatrix();
	GLint pUniform = m_textureProgram.GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_uiSpriteBatch.Begin();

	sprintf_s(buffer, "Num Humans %d", m_humans.size());
	m_spriteFont->draw(m_uiSpriteBatch, buffer, glm::vec2(130, 0),
						glm::vec2(1.0), 0.0f, Gengine::ColorRGBA8(255, 255, 255, 255));

	sprintf_s(buffer, "Num Zombies %d", m_zombies.size());
	m_spriteFont->draw(m_uiSpriteBatch, buffer, glm::vec2(130, 40),
		glm::vec2(1.0), 0.0f, Gengine::ColorRGBA8(255, 255, 255, 255));

	m_uiSpriteBatch.End();
	m_uiSpriteBatch.RenderBatchs();
}

void MainGame::addBlood(const glm::vec2& position, int numParticles) {
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);
	
	glm::vec2 velocity(2.0f, 0.0f);

	Gengine::ColorRGBA8 color(255, 0, 0, 255);

	for (int i = 0; i < numParticles; i++) {
		m_bloodParticleBatch->addParticle(position, glm::rotate(velocity, randAngle(randEngine)), color, 20.0f);
	}
}