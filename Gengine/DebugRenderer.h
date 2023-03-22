#pragma once
#include "Vertex.h"
#include "GLSLProgram.h"
#include <glm/glm.hpp>
#include <vector>

namespace Gengine {
	class DebugRenderer
	{
	public :
		DebugRenderer();
		~DebugRenderer();

		void init();
		void end();
		void drawBox(const glm::vec4& destRect, const Gengine::ColorRGBA8& color, float angle);
		void drawCircle(const glm::vec2& center, const Gengine::ColorRGBA8& color, float radius);
		void render(const glm::mat4& projectionMatrix, float lineWidth);
		void destroy();

		struct DebugVertex {
			glm::vec2 position;
			Gengine::ColorRGBA8 color;
		};

	private :
		Gengine::GLSLProgram m_program;
		std::vector<DebugVertex> m_vertices;
		std::vector<GLuint> m_indices;
		GLuint m_vbo = 0, m_vao = 0, m_ibo = 0;
		int	m_numElements = 0;
	};
}

