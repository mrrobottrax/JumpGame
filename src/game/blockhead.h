#pragma once

#include "entity.h"
#include "window/window.h"

class Blockhead : public Entity
{
public:
	Blockhead(float positionX, float positionY) : Entity()
	{
		this->positionX = positionX;
		this->positionY = positionY;
		this->dimensionsX = Window::LEVEL_WIDTH;
		this->dimensionsY = Window::LEVEL_HEIGHT;
		this->spriteIndex = 1560;
		this->flip = false;
	}

	virtual void tick() override {}
};