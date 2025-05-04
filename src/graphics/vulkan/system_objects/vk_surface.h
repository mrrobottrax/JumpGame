#pragma once

namespace Graphics::Vulkan
{
	inline VkSurfaceKHR vk_surface;

	void CreateSurface();
	void DestroySurface();
}