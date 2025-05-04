#include "pch.h"
#include "vk_atlas_descriptor_set.h"
#include "../vulkan.h"
#include <graphics/vulkan/system_objects/vk_device.h>
#include <graphics/vulkan/game_objects/vk_point_sampler.h>
#include <graphics/vulkan/game_objects/vk_atlas_texture.h>
#include "vk_static_descriptor_pool.h"

namespace Graphics::Vulkan
{
	void CreateAtlasDescriptorSet()
	{
		VkDescriptorSetLayoutBinding samplerBinding{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
			.pImmutableSamplers = &vk_point_sampler,
		};

		VkDescriptorSetLayoutBinding bindings[] = { samplerBinding };

		VkDescriptorSetLayoutCreateInfo layoutInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = _countof(bindings),
			.pBindings = bindings,
		};

		VkAssert(vkCreateDescriptorSetLayout(vk_device, &layoutInfo, nullptr, &vk_atlas_set_layout));

		VkDescriptorSetAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.descriptorPool = vk_static_descriptor_pool,
			.descriptorSetCount = 1,
			.pSetLayouts = &vk_atlas_set_layout,
		};

		VkAssert(vkAllocateDescriptorSets(vk_device, &allocInfo, &vk_atlas_set));

		VkDescriptorImageInfo imageInfo{
			.sampler = vk_point_sampler,
			.imageView = vk_atlas_view,
			.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		};

		VkWriteDescriptorSet write{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = vk_atlas_set,
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.pImageInfo = &imageInfo,
		};

		vkUpdateDescriptorSets(vk_device, 1, &write, 0, nullptr);
	}

	void DestroyAtlasDescriptorSet()
	{
		vkDestroyDescriptorSetLayout(vk_device, vk_atlas_set_layout, nullptr);
	}
}
