#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <Gengine/DebugRenderer.h>

class Capsule
{
public :
	void init(b2World* world,
		const glm::vec2& position,
		const glm::vec2& dimensions,
		float density,
		float friction,
		bool fixedRotation);

	void drawDebug(Gengine::DebugRenderer& debugRenderer);

	b2Body* getBody() const { return (m_body); }
	b2Fixture* getFixture(int idx) const { return (m_fixtures[idx]); }
	const glm::vec2& getDimensions() const { return (m_dimensions); }

private :
	b2Body* m_body = nullptr;
	b2Fixture* m_fixtures[3];
	glm::vec2 m_dimensions;
};

