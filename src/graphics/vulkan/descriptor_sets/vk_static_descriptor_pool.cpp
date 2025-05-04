#include "pch.h"
#include "vk_static_descriptor_pool.h"
#include <graphics/vulkan/vulkan.h>
#include <graphics/vulkan/system_objects/vk_device.h>

namespace Graphics::Vulkan
{
	void create_descriptor_pool()
	{
		VkDescriptorPoolSize poolSize{
			.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = 2,
		};

		VkDescriptorPoolCreateInfo poolInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.maxSets = 2,
			.poolSizeCount = 1,
			.pPoolSizes = &poolSize,
		};

		vk_assert(vkCreateDescriptorPool(vk_device, &poolInfo, nullptr, &vk_static_descriptor_pool));
	}

	void destroy_descriptor_pool()
	{
		vkDestroyDescriptorPool(vk_device, vk_static_descriptor_pool, nullptr);
	}
}
