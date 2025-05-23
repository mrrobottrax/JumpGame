#include "pch.h"
#include "vk_point_sampler.h"
#include <graphics/vulkan/vulkan.h>
#include <graphics/vulkan/system_objects/vk_device.h>

namespace Graphics::Vulkan
{
	void create_point_sampler()
	{
		VkSamplerCreateInfo samplerInfo{
			.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
			.magFilter = VK_FILTER_NEAREST,
			.minFilter = VK_FILTER_NEAREST,
			.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
			.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.anisotropyEnable = VK_FALSE,
			.compareEnable = VK_FALSE,
			.minLod = 0,
			.maxLod = VK_LOD_CLAMP_NONE,
			.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
			.unnormalizedCoordinates = VK_FALSE,
		};

		vk_assert(vkCreateSampler(vk_device, &samplerInfo, nullptr, &vk_point_sampler));
	}

	void destroy_point_sampler()
	{
		vkDestroySampler(vk_device, vk_point_sampler, nullptr);
	}
}
