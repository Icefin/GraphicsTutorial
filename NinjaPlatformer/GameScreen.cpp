#include "GameScreen.h"
#include <Gengine/IMainGame.h>
#include <Gengine/ResourceManager.h>
#include <SDL/SDL.h>
#include <random>

GameScreen::GameScreen(Gengine::Window* window) :
m_window(window) {

}

GameScreen::~GameScreen() {

}

void GameScreen::build() {

}

void GameScreen::destroy() {

}
#include <iostream>
void GameScreen::onEntry() {
	b2Vec2 gravity(0.0f, -25.0f);
	m_world = std::make_unique<b2World>(gravity);

	m_debugRenderer.init();

	//Make the ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -25.0f);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);
	//Make the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Load the textrue
	m_texture = Gengine::ResourceManager::getTexture("Assets/bricks_top.png");

	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(-10.0f, 15.0f);
	std::uniform_real_distribution<float> side(0.5f, 2.5f);
	std::uniform_int_distribution<int> color(0, 255);
	const int NUM_BOXES = 10;

	for (int i = 0; i < NUM_BOXES; i++) {
		Box newBox;

		Gengine::ColorRGBA8 randColor;
		randColor.r = color(randGenerator);
		randColor.g = color(randGenerator);
		randColor.b = color(randGenerator);
		randColor.a = 255;
		newBox.init(m_world.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(side(randGenerator), side(randGenerator)), m_texture, randColor, false);
		m_boxes.push_back(newBox);
	}

	//Initialize Spritebatch
	m_spriteBatch.init();

	//Initialize Shader
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	//Initialize Camera
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(32.0f);

	//Initialize Player
	m_player.init(m_world.get(), glm::vec2(0.0f, 30.0f), glm::vec2(2.0f), glm::vec2(1.0f, 1.8f), Gengine::ColorRGBA8(255, 255, 255, 255));
}

void GameScreen::onExit() {
	m_debugRenderer.destroy();
}

void GameScreen::update() {
	m_camera.update();
	checkInput();

	m_player.update(m_game->inputManager);

	//Update physics simulation
	m_world->Step(1.0f / 60.0f /*frame rate*/, 6, 2);
}

void GameScreen::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_textureProgram.use();

	//Upload texture uniform
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	//Camera Matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_spriteBatch.begin();

	//Draw all the boxes
	for (auto& box : m_boxes) {
		box.draw(m_spriteBatch);
	}
	m_player.draw(m_spriteBatch);

	m_spriteBatch.end();
	m_spriteBatch.renderBatchs();

	m_textureProgram.unuse();

	//Debug rendering
	if (m_debugMode) {
		glm::vec4 destRect;
		for (auto& box : m_boxes) {
			destRect.x = box.getBody()->GetPosition().x - box.getDimensions().x / 2.0f;
			destRect.y = box.getBody()->GetPosition().y - box.getDimensions().y / 2.0f;
			destRect.z = box.getDimensions().x;
			destRect.w = box.getDimensions().y;
			m_debugRenderer.drawBox(destRect, Gengine::ColorRGBA8(255, 255, 255, 255), box.getBody()->GetAngle());
		}
		m_player.drawDebug(m_debugRenderer);
		m_debugRenderer.end();
		m_debugRenderer.render(projectionMatrix, 2.0f);
	}
}

int GameScreen::getNextScreenIndex() const {

	return SCREEN_IDX_NO_SCREEN;
}

int GameScreen::getPrevScreenIndex() const {

	return SCREEN_IDX_NO_SCREEN;
}

void GameScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}
