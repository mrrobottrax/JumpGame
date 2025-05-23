#include "pch.h"
#include "coin.h"
#include "game.h"

constexpr int SPRITE_TIME = 10;

constexpr int STARTING_SPRITE = 3;
constexpr int ENDING_SPRITE = 5;

Coin::Coin(float positionX, float positionY) : Entity()
{
	this->positionX = positionX;
	this->positionY = positionY;
	spriteTimer = SPRITE_TIME;
	spriteIndex = STARTING_SPRITE;
	reverseAnim = false;
}

void Coin::tick()
{
	--spriteTimer;
	if (spriteTimer <= 0)
	{
		spriteTimer = SPRITE_TIME;

		if (reverseAnim)
		{
			if (spriteIndex <= STARTING_SPRITE)
			{
				reverseAnim = false;
				++spriteIndex;
			}
			else
			{
				--spriteIndex;
			}
		}
		else
		{
			if (spriteIndex >= ENDING_SPRITE)
			{
				reverseAnim = true;
				--spriteIndex;
			}
			else
			{
				++spriteIndex;
			}
		}

		flip = reverseAnim;
	}

	bool inX = (int)g_entities[0]->positionX == (int)positionX || (int)g_entities[0]->positionX + 1 == (int)positionX;
	bool inY = (int)g_entities[0]->positionY == (int)positionY || (int)g_entities[0]->positionY + 1 == (int)positionY;
	if (inX && inY)
	{
		delete_entity(this);
		Game::collect_coin();
	}
}
