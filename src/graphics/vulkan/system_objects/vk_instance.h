#pragma once

namespace Graphics::Vulkan
{
	inline VkInstance vk_instance;

	void create_instance();
	void destroy_instance();
}