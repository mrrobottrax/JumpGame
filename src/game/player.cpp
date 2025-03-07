#include "pch.h"
#include "player.h"
#include "input/input.h"
#include "time/time.h"
#include "console/console.h"

constexpr float acceleration = 80;
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
		float sub = acceleration * tickDelta;
		float maxSub = velocityX - wishSpeed;

		velocityX -= fminf(sub, maxSub);
	}
	else if (wishSpeed > velocityX)
	{
		float add = acceleration * tickDelta;
		float maxAdd = wishSpeed - velocityX;

		velocityX += fminf(add, maxAdd);
	}

	velocityX = fmaxf(fminf(velocityX, maxspeed), -maxspeed);

	positionX += velocityX * tickDelta;
}
