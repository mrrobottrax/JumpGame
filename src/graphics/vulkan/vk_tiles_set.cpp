#include "pch.h"
#include "vk_tiles_set.h"
#include "vulkan.h"
#include "vk_device.h"
#include "vk_atlas.h"

void CreateTilesSet()
{
	VkDescriptorSetLayoutBinding binding{
		.binding = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
	};

	VkDescriptorSetLayoutCreateInfo layoutInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = 1,
		.pBindings = &binding,
	};

	VkAssert(vkCreateDescriptorSetLayout(vk_device, &layoutInfo, nullptr, &vk_tiles_set_layout));

	VkDescriptorPoolSize poolSize{
		.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
		.descriptorCount = 1,
	};

	VkDescriptorPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.maxSets = 1,
		.poolSizeCount = 1,
		.pPoolSizes = &poolSize,
	};

	VkAssert(vkCreateDescriptorPool(vk_device, &poolInfo, nullptr, &vk_tiles_set_pool));

	VkDescriptorSetAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = vk_tiles_set_pool,
		.descriptorSetCount = 1,
		.pSetLayouts = &vk_tiles_set_layout,
	};

	VkAssert(vkAllocateDescriptorSets(vk_device, &allocInfo, &vk_tiles_set));

	VkDescriptorImageInfo imageInfo{
		.sampler = VK_NULL_HANDLE,
		.imageView = vk_atlas_view,
		.imageLayout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,
	};

	VkWriteDescriptorSet write{
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = vk_tiles_set,
		.dstBinding = 0,
		.dstArrayElement = 0,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
		.pImageInfo = &imageInfo,
	};

	vkUpdateDescriptorSets(vk_device, 1, &write, 0, nullptr);
}

void DestroyTilesSet()
{
	vkDestroyDescriptorPool(vk_device, vk_tiles_set_pool, nullptr);
	vkDestroyDescriptorSetLayout(vk_device, vk_tiles_set_layout, nullptr);
}
