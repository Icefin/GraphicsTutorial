#pragma once

namespace Gengine {
	class FpsLimiter
	{
	public :
		FpsLimiter();
		~FpsLimiter();

		void init(float maxFPS);
		void setMaxFPS(float maxFPS);

		void begin();
		float end();
	private :
		void calculateFPS();

		float m_fps;
		float m_maxFPS;
		float m_frameTime;
		unsigned int m_startTicks;
	};
}
