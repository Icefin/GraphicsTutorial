#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Gengine {
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void Init(int screenWidth, int screenHeight);

		void Update();

		glm::vec2 ConvertScreenToWorld(glm::vec2 screenCoords);
		bool isBoxInView(const glm::vec2& position, const glm::vec2& dimension);

		void SetScale(float newScale) { _scale = newScale; _needsMatrixUpdate = true; }
		void SetPosition(const glm::vec2& newPosition) { _position = newPosition; _needsMatrixUpdate = true; }

		float GetScale() { return _scale; }
		glm::vec2 GetPosition() { return _position; }
		glm::mat4 GetCameraMatrix() { return _cameraMatrix; }

	private:
		int _screenWidth, _screenHeight;
		bool _needsMatrixUpdate;
		float _scale;
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
	};
}