#pragma once

namespace Graphics::Vulkan
{
	inline VkCommandPool vk_commandpool_main;
	inline VkCommandBuffer vk_commandbuffer_main;

	void create_command_buffers();
	void destroy_command_buffer();
}