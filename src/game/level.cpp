#include "pch.h"
#include "level.h"

bool is_tile_solid(unsigned short tile)
{
	return tile != 0 && tile != 2;
}
