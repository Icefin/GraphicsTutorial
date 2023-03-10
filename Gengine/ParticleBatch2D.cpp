#include "ParticleBatch2D.h"

namespace Gengine {
	void Particle2D::update(float deltaTime) {
		_position += _velocity * deltaTime;
	}

	ParticleBatch2D::ParticleBatch2D() {

	}

	ParticleBatch2D::~ParticleBatch2D() {
		delete[] _particles;
	}

	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture) {
		_maxParticles = maxParticles;
		_particles = new Particle2D[_maxParticles];
		_decayRate = decayRate;
		_texture = texture;
	}

	void ParticleBatch2D::update(float deltaTime) {
		for (int i = 0; i < _maxParticles; i++) {
			if (_particles[i]._life > 0.0f) {
				_particles[i].update(deltaTime);
				_particles[i]._life -= _decayRate * deltaTime;
			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch) {
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < _maxParticles; i++) {
			auto& currParticle = _particles[i];
			if (currParticle._life > 0.0f) {
				glm::vec4 destRect(currParticle._position.x, currParticle._position.y, currParticle._width, currParticle._width);
				spriteBatch->Draw(destRect, uvRect, _texture.id, 0.0f, currParticle._color);
			}
		}
	}

	void ParticleBatch2D::addParticle(const glm::vec2& position, const glm::vec2& velocity, const ColorRGBA8& color, float width) {
		int particleIdx = findFreeParticleIdx();

		auto& currParticle = _particles[particleIdx];

		currParticle._life = 1.0f;
		currParticle._position = position;
		currParticle._velocity = velocity;
		currParticle._color = color;
		currParticle._width = width;
	}

	int ParticleBatch2D::findFreeParticleIdx() {
		for (int idx = _lastFreeParticleIdx; idx < _maxParticles; idx++) {
			if (_particles[idx]._life <= 0.0f) {
				_lastFreeParticleIdx = idx;
				return (idx);
			}
		}

		for (int idx = 0; idx < _lastFreeParticleIdx; idx++) {
			if (_particles[idx]._life <= 0.0f) {
				_lastFreeParticleIdx = idx;
				return (idx);
			}
		}

		return (0);
	}
}