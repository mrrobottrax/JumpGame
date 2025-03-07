#pragma once

class Player
{
public:
	float positionX, positionY;
	float velocityX, velocityY;

	void Tick();
};
inline Player g_player;