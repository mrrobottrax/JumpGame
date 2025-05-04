#include "pch.h"
#include "game.h"
#include "player.h"
#include "coin.h"
#include "blockhead.h"

namespace Game
{
	static int collectedCoins;

	void init()
	{
		add_entity(new Player());
		add_entity(new Coin(2, 8));
		add_entity(new Coin(0, 25));
		add_entity(new Coin(36, 15));
	}

	void shutdown()
	{
		clear_entities();
	}

	void tick()
	{
		for (uint32_t i = 0; i < g_entityCount; ++i)
		{
			g_entities[i]->tick();
		}
	}

	void collect_coin()
	{
		++collectedCoins;

		if (collectedCoins >= 3)
		{
			clear_entities();
			add_entity(new Blockhead(0, 0));
		}
	}
}
