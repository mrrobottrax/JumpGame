#include "pch.h"
#include "entity.h"

void AddEntity(Entity *pEntity)
{
	g_entities[g_entityCount++] = pEntity;
}

void DeleteEntity(Entity *pEntity)
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

void ClearEntities()
{
	for (uint32_t i = 0; i < g_entityCount; ++i)
	{
		delete g_entities[i];
	}

	g_entityCount = 0;
}
