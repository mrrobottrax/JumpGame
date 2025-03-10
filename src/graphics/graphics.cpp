#include <pch.h>
#include "graphics.h"
#include "vulkan/vulkan.h"

void MAGE_InitGraphics()
{
	InitVulkan();
}

void MAGE_EndGraphics()
{
	EndVulkan();
}

void MAGE_WaitForNextFrame()
{
	WaitForFrameVulkan();
}

void MAGE_Render()
{
	RenderFrameVulkan();
}