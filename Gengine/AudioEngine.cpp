#include "AudioEngine.h"
#include "Errors.h"

namespace Gengine {
	void SoundEffect::play(int loops) {
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1) {
			if (Mix_PlayChannel(0, m_chunk, loops) == -1) {
				FatalError("Mix_PlayChannel error : " + std::string(Mix_GetError()));
			}
		}
	}

	void Music::play(int loops) {
		Mix_PlayMusic(m_music, loops);
	}

	void Music::pause() {
		Mix_PauseMusic();
	}

	void Music::resume() {
		Mix_ResumeMusic();
	}

	void Music::stop() {
		Mix_HaltMusic();
	}

	AudioEngine::AudioEngine() {
	
	}

	AudioEngine::~AudioEngine() {
		destroy();
	}

	void AudioEngine::init() {
		//Parameter can be a bitwise combination of MIX_INIT_FAC,
		//MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
			FatalError("Mix_Init error : " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
			FatalError("Mix_OpenAudio error : " + std::string(Mix_GetError()));
		}

		isInitialized = true;
	}

	void AudioEngine::destroy() {
		if (isInitialized) {
			isInitialized = false;
			Mix_Quit();
		}
	}

	SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath) {
		SoundEffect effect;
		
		auto iter = effectMap.find(filePath);
		if (iter == effectMap.end()) {
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr) {
				FatalError("Mix_LoadWAV error : " + std::string(Mix_GetError()));
			}
			effect.m_chunk = chunk;
			effectMap[filePath] = chunk;
		}
		else {
			effect.m_chunk = iter->second;
		}

		return (effect);
	}

	Music AudioEngine::loadMusic(const std::string& filePath) {
		Music music;

		auto iter = musicMap.find(filePath);
		if (iter == musicMap.end()) {
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr) {
				FatalError("Mix_LoadMUS error : " + std::string(Mix_GetError()));
			}
			music.m_music = mixMusic;
			musicMap[filePath] = mixMusic;
		}
		else {
			music.m_music = iter->second;
		}

		return (music);
	}
}