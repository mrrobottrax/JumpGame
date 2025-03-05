#include "pch.h"
#include "vk_instance.h"
#include "vk_physicaldevice.h"
#include "vulkan.h"
#include "exceptions/exceptions.h"

void InitVulkan()
{
	CreateInstance();
	PickPhysicalDevice();
}

void EndVulkan()
{
	DestroyInstance();
}

void VkAssert(VkResult result)
{
	if (result < 0)
	{
		throw VulkanException("Vulkan Error:", result);
	}
}