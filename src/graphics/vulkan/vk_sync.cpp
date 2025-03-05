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

	VkSemaphoreCreateInfo semaphoreInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};

	VkAssert(vkCreateSemaphore(vk_device, &semaphoreInfo, nullptr, &vk_semaphore_rendering));
	VkAssert(vkCreateSemaphore(vk_device, &semaphoreInfo, nullptr, &vk_semaphore_acquireimage));
}

void DestroySyncObjects()
{
	vkDestroySemaphore(vk_device, vk_semaphore_rendering, nullptr);
	vkDestroySemaphore(vk_device, vk_semaphore_acquireimage, nullptr);
	vkDestroyFence(vk_device, vk_fence_main, nullptr);
}
