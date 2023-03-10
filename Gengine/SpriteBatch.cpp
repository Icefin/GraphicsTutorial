#include "SpriteBatch.h"
#include <algorithm>

namespace Gengine {

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) :
		texture(Texture), depth(Depth) {
		topLeft.color = color;
		topLeft.SetPosition(destRect.x, destRect.y + destRect.w);	//x,y : bottomLeftPos, z,w : len of side
		topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.SetPosition(destRect.x, destRect.y);
		bottomLeft.SetUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.SetPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.SetPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle) :
		texture(Texture), depth(Depth) {

		glm::vec2 halfDimension(destRect.z / 2.0f, destRect.w / 2.0f);
		glm::vec2 tl(-halfDimension.x, halfDimension.y);
		glm::vec2 bl(-halfDimension.x, -halfDimension.y);
		glm::vec2 br(halfDimension.x, -halfDimension.y);
		glm::vec2 tr(halfDimension.x, halfDimension.y);

		tl = rotatePoint(tl, angle) + halfDimension;
		bl = rotatePoint(bl, angle) + halfDimension;
		br = rotatePoint(br, angle) + halfDimension;
		tr = rotatePoint(tr, angle) + halfDimension;
		
		topLeft.color = color;
		topLeft.SetPosition(destRect.x + tl.x, destRect.y + tl.y);	//x,y : bottomLeftPos, z,w : len of side
		topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.SetPosition(destRect.x + bl.x, destRect.y + bl.y);
		bottomLeft.SetUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.SetPosition(destRect.x + br.x, destRect.y + br.y);
		bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.SetPosition(destRect.x + tr.x, destRect.y + tr.y);
		topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	glm::vec2 Glyph::rotatePoint(glm::vec2 pos, float angle) {
		glm::vec2 rotVector;
		rotVector.x = pos.x * cos(angle) - pos.y * sin(angle);
		rotVector.y = pos.x * sin(angle) + pos.y * cos(angle);
		return (rotVector);
	}

	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0) {
	
	}
	SpriteBatch::~SpriteBatch() {
	
	}

	void SpriteBatch::Init() {
		CreateVertexArray();
	}

	void SpriteBatch::Begin(GlyphSortType sortType) {
		_sortType = sortType;
		_renderBatches.clear();
		_glyphs.clear();
	}

	void SpriteBatch::End() {
		_glyphsPointers.resize(_glyphs.size());
		for (int i = 0; i < _glyphs.size(); i++) {
			_glyphsPointers[i] = &_glyphs[i];
		}
		SortGlyphs();
		CreateRenderBatches();
	}

	void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle) {
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir) {
		const glm::vec2 xDir(1.0f, 0.0f);
		float angle = acos(glm::dot(xDir, dir));
		if (dir.y < 0.0f) angle = -angle;
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::RenderBatchs() {
		glBindVertexArray(_vao);
		for (int i = 0; i < _renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i]._texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i]._offset, _renderBatches[i]._numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::CreateRenderBatches() {
		std::vector<Vertex> vertices;
		vertices.resize(_glyphsPointers.size() * 6);

		if (_glyphsPointers.empty()) {
			return;
		}

		int offset = 0;
		int currentVertex = 0;
		_renderBatches.emplace_back(offset, 6, _glyphsPointers[0]->texture);
		vertices[currentVertex++] = _glyphsPointers[0]->topLeft;
		vertices[currentVertex++] = _glyphsPointers[0]->bottomLeft;
		vertices[currentVertex++] = _glyphsPointers[0]->bottomRight;
		vertices[currentVertex++] = _glyphsPointers[0]->bottomRight;
		vertices[currentVertex++] = _glyphsPointers[0]->topRight;
		vertices[currentVertex++] = _glyphsPointers[0]->topLeft;
		offset += 6;

		for (int currentGlyph = 1; currentGlyph < _glyphsPointers.size(); currentGlyph++) {
			if (_glyphsPointers[currentGlyph]->texture != _glyphsPointers[currentGlyph - 1]->texture) {
				_renderBatches.emplace_back(offset, 6, _glyphsPointers[currentGlyph]->texture);
			}
			else {
				_renderBatches.back()._numVertices += 6;
			}
			vertices[currentVertex++] = _glyphsPointers[currentGlyph]->topLeft;
			vertices[currentVertex++] = _glyphsPointers[currentGlyph]->bottomLeft;
			vertices[currentVertex++] = _glyphsPointers[currentGlyph]->bottomRight;
			vertices[currentVertex++] = _glyphsPointers[currentGlyph]->bottomRight;
			vertices[currentVertex++] = _glyphsPointers[currentGlyph]->topRight;
			vertices[currentVertex++] = _glyphsPointers[currentGlyph]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr /*orphan the buffer*/, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()); //upload the data

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::CreateVertexArray() {
		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);

		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//Position for attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//ColorRGBA8 for attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	void SpriteBatch::SortGlyphs() {
		switch (_sortType) {
			case GlyphSortType::BACK_TO_FRONT :
				std::stable_sort(_glyphsPointers.begin(), _glyphsPointers.end(), CompareBackToFront);
				break;
			case GlyphSortType::FRONT_TO_BACK:
				std::stable_sort(_glyphsPointers.begin(), _glyphsPointers.end(), CompareFrontToBack);
				break;
			case GlyphSortType::TEXTURE:
				std::stable_sort(_glyphsPointers.begin(), _glyphsPointers.end(), CompareTexture);
				break;
		}
	}

	bool SpriteBatch::CompareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}
	bool SpriteBatch::CompareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);
	}
	bool SpriteBatch::CompareTexture(Glyph* a, Glyph* b) {
		return (a->texture < b->texture);
	}
}