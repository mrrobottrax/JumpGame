#pragma once

namespace Graphics::Vulkan
{
	inline VkDescriptorSet vk_level_set;
	inline VkDescriptorSetLayout vk_level_set_layout;

	void create_level_descriptor_set();
	void destroy_level_descriptor_set();
}