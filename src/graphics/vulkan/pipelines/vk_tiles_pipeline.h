#pragma once

namespace Graphics::Vulkan
{
	inline VkPipeline vk_tiles_pipeline;
	inline VkPipelineLayout vk_tiles_pipeline_layout;

	void CreateTilesPipeline();
	void DestroyTilesPipeline();
}