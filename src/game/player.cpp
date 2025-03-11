#include "pch.h"
#include "player.h"
#include "input/input.h"
#include "time/time.h"
#include "level.h"
#include "window/window.h"

constexpr float acceleration = 100;
constexpr float maxspeed = 10;

constexpr int stepDelay = 10;

constexpr unsigned int IDLE_SPRITE = 24;

constexpr unsigned int STEP0_SPRITE = 25;
constexpr unsigned int STEP1_SPRITE = 26;

constexpr unsigned int JUMP_UP_SPRITE = 27;
constexpr unsigned int JUMP_DOWN_SPRITE = 28;

constexpr unsigned int IMPACT_SPRITE0 = 29;
constexpr unsigned int IMPACT_SPRITE1 = 30;

void Player::Tick()
{
	float wishSpeed = 0;

	if (key_a)
	{
		wishSpeed -= 1;
	}

	if (key_d)
	{
		wishSpeed += 1;
	}

	wishSpeed *= maxspeed;

	if (wishSpeed < velocityX)
	{
		if (wishSpeed < 0 && velocityX > 0)
		{
			velocityX = 0;
		}

		float sub = acceleration * tickDelta;
		float maxSub = velocityX - wishSpeed;

		velocityX -= fminf(sub, maxSub);
	}
	else if (wishSpeed > velocityX)
	{
		if (wishSpeed > 0 && velocityX < 0)
		{
			velocityX = 0;
		}

		float add = acceleration * tickDelta;
		float maxAdd = wishSpeed - velocityX;

		velocityX += fminf(add, maxAdd);
	}

	velocityX = fmaxf(fminf(velocityX, maxspeed), -maxspeed);
	positionX += velocityX * tickDelta;

	velocityY -= 80 * tickDelta;
	positionY += velocityY * tickDelta;

	bool grounded = false;

	if (positionY < 0)
	{
		positionY = 0;
		velocityY = 0;

		grounded = true;
	}

	positionX = fminf(fmaxf(positionX, 0), LEVEL_WIDTH - 1);
	positionY = fminf(fmaxf(positionY, 0), LEVEL_HEIGHT - 1);

	unsigned short gridL = levelData[(int)positionX + (int)(LEVEL_HEIGHT - positionY) * LEVEL_WIDTH];
	unsigned short gridR = levelData[(int)fminf(positionX + 1, LEVEL_WIDTH - 1) + (int)(LEVEL_HEIGHT - positionY) * LEVEL_WIDTH];
	if ((gridL || gridR) && velocityY < 0)
	{
		positionY = ceilf(positionY);
		velocityY = 0;

		grounded = true;
	}

	if (grounded)
	{
		if (key_space)
		{
			velocityY = 30;
		}
	}

	if (grounded)
	{
		if (impactCounter > 0)
		{
			if (impactCounter < 5)
			{
				spriteIndex = IMPACT_SPRITE0;
			}
			else
			{
				spriteIndex = IMPACT_SPRITE1;
			}
			--impactCounter;
		}
		else
		{
			if (abs(velocityX) > 0.01f)
			{
				spriteIndex = walkingLeft ? STEP0_SPRITE : STEP1_SPRITE;
				--stepCounter;
				if (stepCounter <= 0)
				{
					stepCounter = stepDelay;
					walkingLeft = !walkingLeft;
				}
			}
			else
			{
				spriteIndex = IDLE_SPRITE;
			}
		}
	}
	else if (velocityY > 0)
	{
		spriteIndex = JUMP_UP_SPRITE;
	}
	else
	{
		spriteIndex = JUMP_DOWN_SPRITE;
	}

	if (!grounded)
	{
		if (velocityY < 0)
		{
			impactCounter = 6;
		}
		else if (impactCounter == 0)
		{
			impactCounter = 3;
		}
	}

	if (velocityX < 0)
	{
		flip = true;
	}
	else if (velocityX > 0)
	{
		flip = false;
	}
}
