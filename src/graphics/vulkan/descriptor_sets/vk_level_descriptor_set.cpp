#include "pch.h"
#include "vk_level_descriptor_set.h"
#include <graphics/vulkan/game_objects/vk_point_sampler.h>
#include <graphics/vulkan/vulkan.h>
#include <graphics/vulkan/system_objects/vk_device.h>
#include "vk_static_descriptor_pool.h"
#include <graphics/vulkan/game_objects/vk_level_texture.h>

namespace Graphics::Vulkan
{
	void create_level_descriptor_set()
	{
		VkDescriptorSetLayoutBinding samplerBinding{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
		};

		VkDescriptorSetLayoutBinding bindings[] = { samplerBinding };

		VkDescriptorSetLayoutCreateInfo layoutInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = _countof(bindings),
			.pBindings = bindings,
		};

		vk_assert(vkCreateDescriptorSetLayout(vk_device, &layoutInfo, nullptr, &vk_level_set_layout));

		VkDescriptorSetAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.descriptorPool = vk_static_descriptor_pool,
			.descriptorSetCount = 1,
			.pSetLayouts = &vk_level_set_layout,
		};

		vk_assert(vkAllocateDescriptorSets(vk_device, &allocInfo, &vk_level_set));

		VkDescriptorImageInfo imageInfo{
			.sampler = vk_point_sampler,
			.imageView = vk_level_view,
			.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		};

		VkWriteDescriptorSet write{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = vk_level_set,
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.pImageInfo = &imageInfo,
		};

		vkUpdateDescriptorSets(vk_device, 1, &write, 0, nullptr);
	}

	void destroy_level_descriptor_set()
	{
		vkDestroyDescriptorSetLayout(vk_device, vk_level_set_layout, nullptr);
	}
}