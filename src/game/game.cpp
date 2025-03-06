#include "pch.h"
#include "game.h"
#include "input/input.h"
#include "time/time.h"

float posX, posY;

void Game_Update()
{
	if (key_a)
	{
		posX -= tickDelta;
	}

	if (key_d)
	{
		posX += tickDelta;
	}
}