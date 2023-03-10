#pragma once

#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

#include <glm/glm.hpp>

namespace Gengine {
	class Particle2D {
		public :
			friend class ParticleBatch2D;

			void update(float deltaTime);

		private :
			glm::vec2 _position = glm::vec2(0.0f);
			glm::vec2 _velocity = glm::vec2(0.0f);
			ColorRGBA8 _color;
			float _width = 0.0f;
			float _life = 0.0f;
	};

	class ParticleBatch2D
	{
		public :
			friend class Particle;

			ParticleBatch2D();
			~ParticleBatch2D();

			void init(int maxParticles, float decayRate, GLTexture texture);
			void update(float deltaTime);
			void draw(SpriteBatch* spriteBatch);
			void addParticle(const glm::vec2& position, const glm::vec2& velocity, const ColorRGBA8& color, float width);

		private :
			int findFreeParticleIdx();

			Particle2D* _particles = nullptr;
			int _maxParticles = 0;
			int _lastFreeParticleIdx = 0;
			
			GLTexture _texture;
			float _decayRate = 0.1f;
	};
}

