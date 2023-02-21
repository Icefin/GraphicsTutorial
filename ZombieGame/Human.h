#pragma once
#include "Agent.h"

class Human : public Agent
{
public :
	Human();
	virtual ~Human();

	virtual void update();
};

