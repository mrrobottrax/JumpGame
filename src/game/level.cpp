#include "pch.h"
#include "level.h"

bool IsTileSolid(unsigned short tile)
{
	return tile != 0 && tile != 2;
}
