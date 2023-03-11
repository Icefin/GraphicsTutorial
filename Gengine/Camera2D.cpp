#include "Camera2D.h"

namespace Gengine {
	Camera2D::Camera2D() :
		m_screenWidth(500),
		m_screenHeight(500),
		m_needsMatrixUpdate(true),
		m_scale(1.0f),
		m_position(0.0f, 0.0f),
		m_cameraMatrix(1.0f),
		m_orthoMatrix(1.0f) {

	}

	Camera2D::~Camera2D() {

	}

	void Camera2D::init(int screenWidth, int screenHeight) {
		m_screenWidth = screenHeight;
		m_screenHeight = screenHeight;
		m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight);
	}

	void Camera2D::update() {
		if (m_needsMatrixUpdate) {
			glm::vec3 translate(-m_position.x + m_screenWidth / 2, -m_position.y + m_screenHeight / 2, 0.0f);
			m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

			glm::vec3 scale(m_scale, m_scale, 0.0f);
			m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

			m_needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
		screenCoords.y = m_screenHeight - screenCoords.y;
		screenCoords -= glm::vec2(m_screenWidth / 2, m_screenHeight / 2);
		screenCoords /= m_scale;

		screenCoords += m_position;
		return (screenCoords);
	}

	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimension) {
		glm::vec2 scaledScreenDimension = glm::vec2(m_screenWidth, m_screenHeight) / (m_scale * 1.0f);

		const float MIN_DISTANCE_X = dimension.x / 2.0f + scaledScreenDimension.x / 2.0f;
		const float MIN_DISTANCE_Y = dimension.y / 2.0f + scaledScreenDimension.y / 2.0f;

		glm::vec2 centerPosition = position + dimension / 2.0f;
		glm::vec2 centerCameraPosition = m_position;
		glm::vec2 distVec = centerPosition - centerCameraPosition;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		if (xDepth > 0 && yDepth > 0) {
			return (true);
		}
		return (false);
	}
}