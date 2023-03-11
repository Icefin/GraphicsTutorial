#pragma once
#include "Timing.h"
#include <SDL/SDL.h>

namespace Gengine {
	FpsLimiter::FpsLimiter() {

	}
	FpsLimiter::~FpsLimiter() {

	}

	void FpsLimiter::init(float maxFPS) {
		setMaxFPS(maxFPS);
	}
	void FpsLimiter::setMaxFPS(float maxFPS) {
		m_maxFPS = maxFPS;
	}
	void FpsLimiter::begin() {
		m_startTicks = SDL_GetTicks();
	}
	float FpsLimiter::end() {
		calculateFPS();

		float frameTicks = SDL_GetTicks() - m_startTicks;

		//Limit the fps to the max FPS
		if (1000.0f / m_maxFPS > frameTicks) {
			SDL_Delay(1000.0f / m_maxFPS - frameTicks);
		}

		return (m_fps);
	}

	void FpsLimiter::calculateFPS() {
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currFrame = 0;

		static float prevTicks = SDL_GetTicks();

		float currTicks = SDL_GetTicks();

		m_frameTime = currTicks - prevTicks;
		frameTimes[currFrame % NUM_SAMPLES] = m_frameTime;

		prevTicks = currTicks;

		int cnt;

		currFrame++;
		if (currFrame < NUM_SAMPLES) {
			cnt = currFrame;
		}
		else {
			cnt = NUM_SAMPLES;
		}

		float frameTimeAverage = 0;
		for (int i = 0; i < cnt; i++) {
			frameTimeAverage += frameTimes[i];
		}

		frameTimeAverage /= cnt;

		if (frameTimeAverage > 0) {
			m_fps = 1000.0f / frameTimeAverage;
		}
		else {
			m_fps = 60.0f;
		}
	}
}