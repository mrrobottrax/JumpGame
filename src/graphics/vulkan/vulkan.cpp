#include "pch.h"
#include "vk_instance.h"
#include "vk_physicaldevice.h"
#include "vulkan.h"
#include "exceptions/exceptions.h"
#include "vk_device.h"

void InitVulkan()
{
	CreateInstance();
	PickPhysicalDevice();
	CreateDevice();
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