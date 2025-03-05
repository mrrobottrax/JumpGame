#include "pch.h"
#include "vk_sync.h"
#include "vk_device.h"
#include "vulkan.h"

void CreateSyncObjects()
{
	VkFenceCreateInfo fenceInfo{
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT
	};

	VkAssert(vkCreateFence(vk_device, &fenceInfo, nullptr, &vk_fence_main));
}

void DestroySyncObjects()
{
	vkDestroyFence(vk_device, vk_fence_main, nullptr);
}
