#pragma once
#include "Timing.h"
#include <SDL/SDL.h>

namespace Gengine {
	FpsLimiter::FpsLimiter() {

	}
	FpsLimiter::~FpsLimiter() {

	}

	void FpsLimiter::Init(float maxFPS) {
		SetMaxFPS(maxFPS);
	}
	void FpsLimiter::SetMaxFPS(float maxFPS) {
		_maxFPS = maxFPS;
	}
	void FpsLimiter::Begin() {
		_startTicks = SDL_GetTicks();
	}
	float FpsLimiter::End() {
		calculateFPS();

		float frameTicks = SDL_GetTicks() - _startTicks;

		//Limit the fps to the max FPS
		if (1000.0f / _maxFPS > frameTicks) {
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		}

		return _fps;
	}

	void FpsLimiter::calculateFPS() {
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currFrame = 0;

		static float prevTicks = SDL_GetTicks();

		float currTicks = SDL_GetTicks();

		_frameTime = currTicks - prevTicks;
		frameTimes[currFrame % NUM_SAMPLES] = _frameTime;

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
			_fps = 1000.0f / frameTimeAverage;
		}
		else {
			_fps = 60.0f;
		}
	}
}