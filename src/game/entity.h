#pragma once

class Entity
{
public:
	bool flip;
	int spriteIndex;
	float positionX, positionY;

	Entity() : flip(false), spriteIndex(1), positionX(0), positionY(0)
	{}

	virtual void Tick() = 0;
};

inline uint32_t g_entityCount;
inline Entity* g_entities[32];

void AddEntity(Entity *pEntity);
void DeleteEntity(Entity *pEntity);
void ClearEntities();