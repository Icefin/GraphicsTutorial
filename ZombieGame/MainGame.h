#pragma once
#include "Level.h"
#include "Player.h"
#include "Bullet.h"

#include <Gengine/Window.h>
#include <Gengine/GLSLProgram.h>
#include <Gengine/Camera2D.h>
#include <Gengine/InputManager.h>
#include <Gengine/SpriteBatch.h>
#include <Gengine/SpriteFont.h>
#include <Gengine/AudioEngine.h>
#include <Gengine/ParticleEngine2D.h>
#include <Gengine/ParticleBatch2D.h>

class Zombie;

enum class GameState {
	PLAY,
	EXIT
};

class MainGame
{
public :
	MainGame();
	~MainGame();

	void run();

private :
	void initSystem();
	void initShader();
	void initLevel();
	void gameLoop();
	void updateAgents(float deltaTime);
	void updateBullets(float deltaTime);
	void checkVictory();
	void processInput();
	void drawGame();
	void drawUI();

	void addBlood(const glm::vec2& position, int numParticles);

	Gengine::Window m_window;
	Gengine::GLSLProgram m_textureProgram;
	Gengine::InputManager m_inputManager;
	Gengine::Camera2D m_camera;
	Gengine::Camera2D m_uiCamera;
	Gengine::SpriteBatch m_agentSpriteBatch;
	Gengine::SpriteBatch m_uiSpriteBatch;
	Gengine::AudioEngine m_audioEngine;
	Gengine::ParticleEngine2D m_particleEngine2D;
	Gengine::ParticleBatch2D* m_bloodParticleBatch;
	Gengine::SpriteFont* m_spriteFont;
	std::vector<Level*> m_levels;

	int m_screenWidth, m_screenHeight;

	GameState m_gameState;
	float m_fps;
	int m_currentLevel;

	int m_numHumansKilled;
	int m_numZombiesKilled;

	std::vector<Human*> m_humans;
	std::vector<Zombie*> m_zombies;
	std::vector<Bullet> m_bullets;

	Player* m_player;
};