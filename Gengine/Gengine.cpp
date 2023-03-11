//// Gengine.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
////
//
#include "pch.h"
//#include "framework.h"
//
//// TODO: 라이브러리 함수의 예제입니다.
//void fnGengine()
//{
//}

#include <SDL/SDL.h>
#include <GL/glew.h>

#include "Gengine.h"

namespace Gengine {
	int init() {
		//Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		//Tell SDL that we want a double buffered window so we don't get any flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return (0);
	}
}