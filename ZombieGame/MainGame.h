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

	Gengine::Window _window;
	Gengine::GLSLProgram _textureProgram;
	Gengine::InputManager _inputManager;
	Gengine::Camera2D _camera;
	Gengine::Camera2D _uiCamera;
	Gengine::SpriteBatch _agentSpriteBatch;
	Gengine::SpriteBatch _uiSpriteBatch;
	Gengine::AudioEngine _audioEngine;
	Gengine::ParticleEngine2D _particleEngine2D;
	Gengine::ParticleBatch2D* _bloodParticleBatch;
	Gengine::SpriteFont* _spriteFont;
	std::vector<Level*> _levels;

	int _screenWidth, _screenHeight;

	GameState _gameState;
	float _fps;
	int _currentLevel;

	int _numHumansKilled;
	int _numZombiesKilled;

	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;
	std::vector<Bullet> _bullets;

	Player* _player;
};