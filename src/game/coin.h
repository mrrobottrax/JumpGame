#pragma once

#include "entity.h"

class Coin : public Entity
{
	int spriteTimer;
	bool reverseAnim;

public:
	Coin(float positionX, float positionY);

	virtual void tick() override;
};