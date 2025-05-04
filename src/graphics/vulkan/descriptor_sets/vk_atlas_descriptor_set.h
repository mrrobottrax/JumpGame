#pragma once

namespace Graphics::Vulkan
{
	inline VkDescriptorSet vk_atlas_set;
	inline VkDescriptorSetLayout vk_atlas_set_layout;

	void create_atlas_descriptor_set();
	void destroy_atlas_descriptor_set();
}