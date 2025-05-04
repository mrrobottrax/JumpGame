#pragma once

namespace Graphics::Vulkan
{
	inline VkInstance vk_instance;

	void CreateInstance();
	void DestroyInstance();
}