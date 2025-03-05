#include "pch.h"
#include "exceptions/exceptions.h"
#include "vulkan.h"
#include "vk_instance.h"
#include "vk_physicaldevice.h"
#include "vk_device.h"
#include "vk_debug.h"
#include "vk_surface.h"

void InitVulkan()
{
	CreateInstance();
	PickPhysicalDevice();
	CreateDevice();
	CreateSurface();
}

void EndVulkan()
{
	DestroyDevice();
	DestroyInstance();
}

void VkAssert(VkResult result)
{
	if (result < 0)
	{
		throw VulkanException("Vulkan Error:", result);
	}
}