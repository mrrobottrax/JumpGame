#pragma once

namespace Graphics::Vulkan
{
	inline VkSurfaceKHR vk_surface;

	void create_surface();
	void destroy_surface();
}