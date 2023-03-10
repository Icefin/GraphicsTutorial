#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Vertex.h"

namespace Gengine {
	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Glyph {
		public :
			Glyph() {};
			Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color);
			Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle);

			GLuint texture;
			float depth;

			Vertex topLeft;
			Vertex bottomLeft;
			Vertex topRight;
			Vertex bottomRight;
		
		private :
			glm::vec2 rotatePoint(glm::vec2 pos, float angle);

	};

	class RenderBatch {
	public :
		RenderBatch(GLuint offset, GLuint numVertices, GLuint texture) : _offset(offset), _numVertices(numVertices), _texture(texture) {};
		GLuint _offset;
		GLuint _numVertices;
		GLuint _texture;
	};

	class SpriteBatch
	{
	public :
		SpriteBatch();
		~SpriteBatch();

		void Init();
		void Begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void End();

		void Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
		void Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);
		void Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);
		void RenderBatchs();
	private :
		void CreateRenderBatches();
		void CreateVertexArray();
		void SortGlyphs();

		static bool CompareFrontToBack(Glyph* a, Glyph* b);
		static bool CompareBackToFront(Glyph* a, Glyph* b);
		static bool CompareTexture(Glyph* a, Glyph* b);

		GLuint _vbo;
		GLuint _vao;

		GlyphSortType _sortType;

		std::vector<Glyph*> _glyphsPointers;
		std::vector<Glyph> _glyphs;
		std::vector<RenderBatch> _renderBatches;
	};
}