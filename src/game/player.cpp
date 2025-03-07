#include "pch.h"
#include "player.h"
#include "input/input.h"
#include "time/time.h"

constexpr float acceleration = 100;
constexpr float maxspeed = 10;

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

	if (positionY < 0)
	{
		positionY = 0;
		velocityY = 0;

		if (key_space)
		{
			velocityY = 30;
		}
	}
}
