#pragma once

namespace Graphics::Vulkan
{
	inline VkPipeline vk_tiles_pipeline;
	inline VkPipelineLayout vk_tiles_pipeline_layout;

	void create_tile_pipeline();
	void destroy_tiles_pipeline();
}