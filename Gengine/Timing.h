#pragma once

namespace Gengine {
	class FpsLimiter
	{
	public :
		FpsLimiter();
		~FpsLimiter();

		void Init(float maxFPS);
		void SetMaxFPS(float maxFPS);

		void Begin();
		float End();
	private :
		void calculateFPS();

		float _fps;
		float _maxFPS;
		float _frameTime;
		unsigned int _startTicks;
	};
}
