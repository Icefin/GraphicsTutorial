#pragma once
#include <unordered_map>

#include <glm/glm.hpp>

namespace Gengine {
	class InputManager
	{
	public :
		InputManager();
		~InputManager();

		void update();

		void PressKey(unsigned int keyID);
		void ReleaseKey(unsigned int keyID);

		void SetMouseCoords(float x, float y);
		
		//Returns true if the key is held down
		bool isKeyDown(unsigned int keyID);
		//Returns true if the key was just pressed
		bool isKeyPressed(unsigned int keyID);

		glm::vec2 GetMouseCoords() const { return _mouseCoords; }

	private :
		bool wasKeyDown(unsigned int keyID);

		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _prevKeyMap;
		glm::vec2 _mouseCoords;
	};
}
