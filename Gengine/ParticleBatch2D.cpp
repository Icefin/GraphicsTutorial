#include "ParticleBatch2D.h"

namespace Gengine {

	ParticleBatch2D::ParticleBatch2D() {

	}

	ParticleBatch2D::~ParticleBatch2D() {
		delete[] _particles;
	}

	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D&, float)> updateFunc) {
		_maxParticles = maxParticles;
		_particles = new Particle2D[_maxParticles];
		_decayRate = decayRate;
		_texture = texture;
		_updateFunc = updateFunc;
	}

	void ParticleBatch2D::update(float deltaTime) {
		for (int i = 0; i < _maxParticles; i++) {
			if (_particles[i].life > 0.0f) {
				_updateFunc(_particles[i], deltaTime);
				_particles[i].life -= _decayRate * deltaTime;
			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch) {
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < _maxParticles; i++) {
			auto& currParticle = _particles[i];
			if (currParticle.life > 0.0f) {
				glm::vec4 destRect(currParticle.position.x, currParticle.position.y, currParticle.width, currParticle.width);
				spriteBatch->Draw(destRect, uvRect, _texture.id, 0.0f, currParticle.color);
			}
		}
	}

	void ParticleBatch2D::addParticle(const glm::vec2& position, const glm::vec2& velocity, const ColorRGBA8& color, float width) {
		int particleIdx = findFreeParticleIdx();

		auto& currParticle = _particles[particleIdx];

		currParticle.life = 1.0f;
		currParticle.position = position;
		currParticle.velocity = velocity;
		currParticle.color = color;
		currParticle.width = width;
	}

	int ParticleBatch2D::findFreeParticleIdx() {
		for (int idx = _lastFreeParticleIdx; idx < _maxParticles; idx++) {
			if (_particles[idx].life <= 0.0f) {
				_lastFreeParticleIdx = idx;
				return (idx);
			}
		}

		for (int idx = 0; idx < _lastFreeParticleIdx; idx++) {
			if (_particles[idx].life <= 0.0f) {
				_lastFreeParticleIdx = idx;
				return (idx);
			}
		}

		return (0);
	}
}