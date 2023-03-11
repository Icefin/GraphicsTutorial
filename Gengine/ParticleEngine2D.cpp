#include "ParticleEngine2D.h"
#include "ParticleBatch2D.h"
#include "SpriteBatch.h"


namespace Gengine {
	ParticleEngine2D::ParticleEngine2D() {

	}

	ParticleEngine2D::~ParticleEngine2D() {
		for (auto& batch : m_particleBatches) {
			delete batch;
		}
	}

	void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particleBatch) {
		m_particleBatches.push_back(particleBatch);
	}

	void ParticleEngine2D::update(float deltaTime) {
		for (auto& batch : m_particleBatches) {
			batch->update(deltaTime);
		}
	}

	void ParticleEngine2D::draw(SpriteBatch* spriteBatch) {
		for (auto& batch : m_particleBatches) {
			spriteBatch->begin();

			batch->draw(spriteBatch);

			spriteBatch->end();
			spriteBatch->renderBatchs();
		}
	}
}