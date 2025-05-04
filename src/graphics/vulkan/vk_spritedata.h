#pragma once

namespace Graphics::Vulkan
{
	struct SpriteData
	{
		unsigned short positionX, positionY;
		short spriteIndex;
		unsigned char sizeX, sizeY;
	};
}