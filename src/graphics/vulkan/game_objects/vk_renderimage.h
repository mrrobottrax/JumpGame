#pragma once

#include <graphics/vulkan/vk_memory.h>

namespace Graphics::Vulkan
{
	inline VkImage vk_render_image;
	inline VkImageView vk_render_image_view;
	inline VkFramebuffer vk_render_framebuffer;

	inline MemoryAllocation vk_render_image_memory;

	void CreateRenderImage();
	void CreateRenderImageView();
	void DestroyRenderImage();
}