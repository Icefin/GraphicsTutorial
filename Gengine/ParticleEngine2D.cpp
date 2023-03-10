#include "ParticleEngine2D.h"
#include "ParticleBatch2D.h"
#include "SpriteBatch.h"


namespace Gengine {
	ParticleEngine2D::ParticleEngine2D() {

	}

	ParticleEngine2D::~ParticleEngine2D() {
		for (auto& batch : _particleBatches) {
			delete batch;
		}
	}

	void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particleBatch) {
		_particleBatches.push_back(particleBatch);
	}

	void ParticleEngine2D::update(float deltaTime) {
		for (auto& batch : _particleBatches) {
			batch->update(deltaTime);
		}
	}

	void ParticleEngine2D::draw(SpriteBatch* spriteBatch) {
		for (auto& batch : _particleBatches) {
			spriteBatch->Begin();

			batch->draw(spriteBatch);

			spriteBatch->End();
			spriteBatch->RenderBatchs();
		}
	}
}