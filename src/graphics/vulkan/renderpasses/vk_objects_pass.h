#pragma once

namespace Graphics::Vulkan
{
	inline VkRenderPass vk_objects_pass;

	void CreateObjectsPass();
	void DestroyObjectsPass();
}