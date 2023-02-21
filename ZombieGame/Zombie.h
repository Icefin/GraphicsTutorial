#pragma once
#include "Agent.h"

class Zombie : public Agent
{
public :
	Zombie();
	~Zombie();

	virtual void update();
};

