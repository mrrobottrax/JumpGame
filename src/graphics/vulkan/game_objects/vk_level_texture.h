#pragma once

#include <graphics/vulkan/vk_memory.h>

namespace Graphics::Vulkan
{
	inline VkImage vk_level_image;
	inline VkImageView vk_level_view;

	inline MemoryAllocation vk_level_memory;

	void CreateLevelImage();
	void CreateLevelImageView();
	void DestroyLevelImage();
}