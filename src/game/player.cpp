#include "pch.h"
#include "player.h"
#include "input/input.h"
#include "time/time.h"
#include "level.h"
#include "window/window.h"

constexpr float ACCELERATION = 100;
constexpr float MAX_SPEED = 10;
constexpr float JUMP_VELOCITY = 25;

constexpr int STEP_DELAY = 10;
constexpr int COYOTE_TIME = 7;

constexpr unsigned int IDLE_SPRITE = 120;

constexpr unsigned int STEP0_SPRITE = 121;
constexpr unsigned int STEP1_SPRITE = 122;

constexpr unsigned int JUMP_UP_SPRITE = 123;
constexpr unsigned int JUMP_DOWN_SPRITE = 124;

constexpr unsigned int IMPACT_SPRITE0 = 125;
constexpr unsigned int IMPACT_SPRITE1 = 126;

Player::Player()
{
	stepCounter = 0;
	impactCounter = 0;
	walkingLeft = false;
	coyoteCounter = 0;
	velocityX = 0;
	velocityY = 0;
	jumpHeldLastFrame = false;
}

void Player::tick()
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

	wishSpeed *= MAX_SPEED;

	if (wishSpeed < velocityX)
	{
		if (wishSpeed < 0 && velocityX > 0)
		{
			velocityX = 0;
		}

		float sub = ACCELERATION * tickDelta;
		float maxSub = velocityX - wishSpeed;

		velocityX -= fminf(sub, maxSub);
	}
	else if (wishSpeed > velocityX)
	{
		if (wishSpeed > 0 && velocityX < 0)
		{
			velocityX = 0;
		}

		float add = ACCELERATION * tickDelta;
		float maxAdd = wishSpeed - velocityX;

		velocityX += fminf(add, maxAdd);
	}

	velocityX = fmaxf(fminf(velocityX, MAX_SPEED), -MAX_SPEED);
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

	if (positionY >= LEVEL_HEIGHT - 1)
	{
		positionY = LEVEL_HEIGHT - 1;
		velocityY = 0;
	}

	positionX = fminf(fmaxf(positionX, 0), LEVEL_WIDTH - 1);

	unsigned short gridL = LEVEL_DATA[(int)positionX + (int)(LEVEL_HEIGHT - positionY) * LEVEL_WIDTH];
	unsigned short gridR = LEVEL_DATA[(int)fminf(positionX + 1, LEVEL_WIDTH - 1) + (int)(LEVEL_HEIGHT - positionY) * LEVEL_WIDTH];
	if ((is_tile_solid(gridL) || is_tile_solid(gridR)) && velocityY < 0)
	{
		positionY = ceilf(positionY);
		velocityY = 0;

		grounded = true;
	}

	if (!grounded)
	{
		if (coyoteCounter > 0)
			--coyoteCounter;
	}
	else
	{
		coyoteCounter = COYOTE_TIME;
	}

	if (coyoteCounter > 0)
	{
		grounded = 1;
	}

	if (grounded)
	{
		if (key_space && !jumpHeldLastFrame)
		{
			velocityY = JUMP_VELOCITY;
			coyoteCounter = 0;
		}

		if (key_space)
		{
			jumpHeldLastFrame = true;
		}

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
					stepCounter = STEP_DELAY;
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

	if (!key_space)
	{
		jumpHeldLastFrame = false;
	}
}
