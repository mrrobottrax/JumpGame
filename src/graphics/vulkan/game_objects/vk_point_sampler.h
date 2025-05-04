#pragma once

namespace Graphics::Vulkan
{
	inline VkSampler vk_point_sampler;

	void create_point_sampler();
	void destroy_point_sampler();
}