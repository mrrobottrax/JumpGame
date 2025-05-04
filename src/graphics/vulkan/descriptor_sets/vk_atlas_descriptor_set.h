#pragma once

namespace Graphics::Vulkan
{
	inline VkDescriptorSet vk_atlas_set;
	inline VkDescriptorSetLayout vk_atlas_set_layout;

	void CreateAtlasDescriptorSet();
	void DestroyAtlasDescriptorSet();
}