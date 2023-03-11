#pragma once

#include <SDL/SDL_mixer.h>
#include <string>
#include <map>

namespace Gengine {
	class SoundEffect {
		public :
			friend class AudioEngine;

			/// <summary>
			/// Plays the effect file
			/// @Param loops : If loops == -1, loop forever,
			/// otherwise play it loops + 1 times
			/// </summary>
			/// <param name="loops"></param>
			void play(int loops = 0);
		
		private :
			Mix_Chunk* m_chunk = nullptr;
	};
	
	class Music {
		public :
			friend class AudioEngine;

			/// <summary>
			/// Plays the music file
			/// @Param loops : If loops == -1, loop forever,
			/// otherwise play it loops times
			/// </summary>
			/// <param name="loops"></param>
			void play(int loops = -1);
			static void pause();
			static void resume();
			static void stop();

		private :
			Mix_Music* m_music = nullptr;
	};

	class AudioEngine
	{
		public :
			AudioEngine();
			~AudioEngine();

			void init();
			void destroy();

			SoundEffect loadSoundEffect(const std::string& filePath);
			Music loadMusic(const std::string& filePath);

		private :
			bool m_isInitialized = false;

			std::map<std::string, Mix_Chunk*> m_effectMap;
			std::map<std::string, Mix_Music*> m_musicMap;

	};
}