#pragma once

namespace Graphics::Vulkan
{
	struct QueueFamilyIndices
	{
		uint32_t mainQueueFamily;
	};
	inline QueueFamilyIndices vk_queue_family_indices;

	void get_queue_families();
}