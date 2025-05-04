#pragma once

namespace Graphics::Vulkan
{
	inline VkDescriptorPool vk_static_descriptor_pool;

	void CreateDescriptorPool();
	void DestroyDescriptorPool();
}