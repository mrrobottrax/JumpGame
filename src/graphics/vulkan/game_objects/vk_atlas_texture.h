#pragma once
#include <graphics/vulkan/vk_memory.h>

namespace Graphics::Vulkan
{
	inline VkImage vk_atlas_image;
	inline VkImageView vk_atlas_view;

	inline MemoryAllocation vk_atlas_memory;

	void CreateAtlasTexture();
	void LoadAtlasTexture();
	void DestroyAtlasTexture();
}