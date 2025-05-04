#pragma once

class Entity
{
public:
	float positionX, positionY;
	short spriteIndex;
	unsigned char dimensionsX, dimensionsY;
	bool flip;

	Entity() : flip(false), spriteIndex(1), positionX(0), positionY(0), dimensionsX(0), dimensionsY(0)
	{}

	virtual void tick() = 0;
};

inline uint32_t g_entityCount;
inline Entity* g_entities[32];

void add_entity(Entity *pEntity);
void delete_entity(Entity *pEntity);
void clear_entities();