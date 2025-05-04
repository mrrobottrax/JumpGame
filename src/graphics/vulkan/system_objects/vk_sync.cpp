#include "pch.h"
#include "vk_sync.h"
#include "vk_device.h"
#include <graphics/vulkan/vulkan.h>

namespace Graphics::Vulkan
{
	void create_sync_objects()
	{
		VkFenceCreateInfo fenceInfo{
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT
		};

		vk_assert(vkCreateFence(vk_device, &fenceInfo, nullptr, &vk_fence_main));

		VkSemaphoreCreateInfo semaphoreInfo{
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		};

		vk_assert(vkCreateSemaphore(vk_device, &semaphoreInfo, nullptr, &vk_semaphore_rendering));
		vk_assert(vkCreateSemaphore(vk_device, &semaphoreInfo, nullptr, &vk_semaphore_acquireimage));
	}

	void destroy_sync_objects()
	{
		vkDestroySemaphore(vk_device, vk_semaphore_rendering, nullptr);
		vkDestroySemaphore(vk_device, vk_semaphore_acquireimage, nullptr);
		vkDestroyFence(vk_device, vk_fence_main, nullptr);
	}
}
