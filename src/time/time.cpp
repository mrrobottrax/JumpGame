#include "pch.h"
#include "time.h"
#include "game/game.h"

using namespace std::chrono;

static system_clock::time_point nextTick = system_clock::now();
void MAGE_UpdateTime()
{
	auto current = system_clock::now();

	while (current > nextTick)
	{
		constexpr system_clock::duration delta((int)(10000000.0 / ticksPerSecond + 0.5));
		nextTick += delta;

		Game_Update();
	}
}