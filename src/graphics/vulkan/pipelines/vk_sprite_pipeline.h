#pragma once

namespace Graphics::Vulkan
{
	inline VkPipeline vk_sprite_pipeline;
	inline VkPipelineLayout vk_sprite_pipeline_layout;

	void create_sprite_pipeline();
	void destroy_sprite_pipeline();
}