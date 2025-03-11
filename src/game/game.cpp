#include "pch.h"
#include "game.h"
#include "player.h"
#include "coin.h"

void Game_Init()
{
	AddEntity(new Player());
	AddEntity(new Coin(2, 8));
	AddEntity(new Coin(5, 23));
	AddEntity(new Coin(37, 14));
}

void Game_End()
{
	ClearEntities();
}

void Game_Tick()
{
	for (uint32_t i = 0; i < g_entityCount; ++i)
	{
		g_entities[i]->Tick();
	}
}