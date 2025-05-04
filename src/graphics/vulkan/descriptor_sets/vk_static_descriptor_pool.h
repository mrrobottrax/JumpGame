#pragma once

namespace Graphics::Vulkan
{
	inline VkDescriptorPool vk_static_descriptor_pool;

	void create_descriptor_pool();
	void destroy_descriptor_pool();
}