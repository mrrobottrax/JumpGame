#pragma once

#include "Entity.h"

class Player : public Entity
{
	int stepCounter;
	int impactCounter;
	bool walkingLeft;
	int coyoteCounter;
	bool jumpHeldLastFrame;

public:
	float velocityX, velocityY;

	Player();

	virtual void tick() override;
};