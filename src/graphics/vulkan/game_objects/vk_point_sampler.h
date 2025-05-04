#pragma once

namespace Graphics::Vulkan
{
	inline VkSampler vk_point_sampler;

	void CreatePointSampler();
	void DestroyPointSampler();
}