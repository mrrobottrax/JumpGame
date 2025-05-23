#pragma once
#include <graphics/vulkan/vk_memory.h>

namespace Graphics::Vulkan
{
	inline VkImage vk_win_image;
	inline VkImageView vk_win_view;

	inline MemoryAllocation vk_win_memory;

	void create_win_texture();
	void load_win_texture();
	void destroy_win_texture();
}