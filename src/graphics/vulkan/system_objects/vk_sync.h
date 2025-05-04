#pragma once

namespace Graphics::Vulkan
{
	inline VkFence vk_fence_main;
	inline VkSemaphore vk_semaphore_rendering;
	inline VkSemaphore vk_semaphore_acquireimage;

	void create_sync_objects();
	void destroy_sync_objects();
}