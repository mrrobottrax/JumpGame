#include "pch.h"
#include "vk_descriptor_set.h"
#include "vk_device.h"
#include "vulkan.h"
#include "vk_unform_buffer.h"

void CreateDescriptorSet()
{
	// Create layout

	VkDescriptorSetLayoutBinding layoutBinding{
		.binding = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
	};

	VkDescriptorSetLayoutCreateInfo setInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = 1,
		.pBindings = &layoutBinding,
	};

	VkAssert(vkCreateDescriptorSetLayout(vk_device, &setInfo, nullptr, &vk_set_layout));

	// Create pool

	VkDescriptorPoolSize poolSize{
		.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
	};

	VkDescriptorPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.maxSets = 1,
		.poolSizeCount = 1,
		.pPoolSizes = &poolSize,
	};

	VkAssert(vkCreateDescriptorPool(vk_device, &poolInfo, nullptr, &vk_descriptor_pool));

	VkDescriptorSetAllocateInfo allocateInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = vk_descriptor_pool,
		.descriptorSetCount = 1,
		.pSetLayouts = &vk_set_layout,
	};

	VkAssert(vkAllocateDescriptorSets(vk_device, &allocateInfo, &vk_descriptor_set));

	VkDescriptorBufferInfo bufferInfo{
		.buffer = vk_uniform_buffer,
		.offset = 0,
		.range = sizeof(ObjectData),
	};

	VkWriteDescriptorSet write{
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = vk_descriptor_set,
		.dstBinding = 0,
		.dstArrayElement = 0,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.pBufferInfo = &bufferInfo,
	};

	vkUpdateDescriptorSets(vk_device, 1, &write, 0, nullptr);
}

void DestroyDescriptorSet()
{
	vkDestroyDescriptorSetLayout(vk_device, vk_set_layout, nullptr);
	vkDestroyDescriptorPool(vk_device, vk_descriptor_pool, nullptr);
}
