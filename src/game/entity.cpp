#include "pch.h"
#include "entity.h"

void add_entity(Entity *pEntity)
{
	g_entities[g_entityCount++] = pEntity;
}

void delete_entity(Entity *pEntity)
{
	for (uint32_t i = 0; i < g_entityCount; ++i)
	{
		if (g_entities[i] == pEntity)
		{
			delete g_entities[i];
			g_entities[i] = g_entities[--g_entityCount];
			return;
		}
	}
}

void clear_entities()
{
	for (uint32_t i = 0; i < g_entityCount; ++i)
	{
		delete g_entities[i];
	}

	g_entityCount = 0;
}
