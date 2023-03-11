#include "ParticleBatch2D.h"

namespace Gengine {

	ParticleBatch2D::ParticleBatch2D() {

	}

	ParticleBatch2D::~ParticleBatch2D() {
		delete[] m_particles;
	}

	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D&, float)> updateFunc) {
		m_maxParticles = maxParticles;
		m_particles = new Particle2D[m_maxParticles];
		m_decayRate = decayRate;
		m_texture = texture;
		m_updateFunc = updateFunc;
	}

	void ParticleBatch2D::update(float deltaTime) {
		for (int i = 0; i < m_maxParticles; i++) {
			if (m_particles[i].life > 0.0f) {
				m_updateFunc(m_particles[i], deltaTime);
				m_particles[i].life -= m_decayRate * deltaTime;
			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch) {
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < m_maxParticles; i++) {
			auto& currParticle = m_particles[i];
			if (currParticle.life > 0.0f) {
				glm::vec4 destRect(currParticle.position.x, currParticle.position.y, currParticle.width, currParticle.width);
				spriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, currParticle.color);
			}
		}
	}

	void ParticleBatch2D::addParticle(const glm::vec2& position, const glm::vec2& velocity, const ColorRGBA8& color, float width) {
		int particleIdx = findFreeParticleIdx();

		auto& currParticle = m_particles[particleIdx];

		currParticle.life = 1.0f;
		currParticle.position = position;
		currParticle.velocity = velocity;
		currParticle.color = color;
		currParticle.width = width;
	}

	int ParticleBatch2D::findFreeParticleIdx() {
		for (int idx = m_lastFreeParticleIdx; idx < m_maxParticles; idx++) {
			if (m_particles[idx].life <= 0.0f) {
				m_lastFreeParticleIdx = idx;
				return (idx);
			}
		}

		for (int idx = 0; idx < m_lastFreeParticleIdx; idx++) {
			if (m_particles[idx].life <= 0.0f) {
				m_lastFreeParticleIdx = idx;
				return (idx);
			}
		}

		return (0);
	}
}