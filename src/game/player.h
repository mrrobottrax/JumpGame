#pragma once

class Player
{
	int stepCounter;
	int impactCounter;
	bool walkingLeft;

public:
	bool flip;
	int spriteIndex;
	float positionX, positionY;
	float velocityX, velocityY;

	void Tick();
};
inline Player g_player;