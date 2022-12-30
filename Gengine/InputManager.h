#pragma once
#include <unordered_map>

namespace Gengine {
	class InputManager
	{
	public :
		InputManager();
		~InputManager();

		void PressKey(unsigned int keyID);
		void ReleaseKey(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

	private :
		std::unordered_map<unsigned int, bool> _keyMap;
	};
}
