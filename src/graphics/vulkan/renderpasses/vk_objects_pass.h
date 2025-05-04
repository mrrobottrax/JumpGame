#pragma once

namespace Graphics::Vulkan
{
	inline VkRenderPass vk_objects_pass;

	void create_objects_pass();
	void destroy_objects_pass();
}