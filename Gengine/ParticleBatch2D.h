#pragma once

#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

#include <glm/glm.hpp>

#include <functional>

namespace Gengine {

	class Particle2D {
		public :
			glm::vec2 position = glm::vec2(0.0f);
			glm::vec2 velocity = glm::vec2(0.0f);
			ColorRGBA8 color;
			float width = 0.0f;
			float life = 0.0f;
	};

	inline void defaultParticleUpdate(Particle2D& particle, float deltaTime) {
		particle.position += particle.velocity * deltaTime;
	}

	class ParticleBatch2D
	{
		public :
			friend class Particle;

			ParticleBatch2D();
			~ParticleBatch2D();

			void init(int maxParticles, float decayRate, GLTexture texture,
					std::function<void(Particle2D&, float)> updateFunc = defaultParticleUpdate);
			void update(float deltaTime);
			void draw(SpriteBatch* spriteBatch);
			void addParticle(const glm::vec2& position, const glm::vec2& velocity, const ColorRGBA8& color, float width);

		private :
			int findFreeParticleIdx();

			Particle2D* m_particles = nullptr;
			int m_maxParticles = 0;
			int m_lastFreeParticleIdx = 0;
			
			GLTexture m_texture;
			float m_decayRate = 0.1f;
			std::function<void(Particle2D&, float)> m_updateFunc;
	};
}

