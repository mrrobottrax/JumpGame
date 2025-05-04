#pragma once

namespace Graphics::Vulkan
{
	inline VkPipeline vk_sprite_pipeline;
	inline VkPipelineLayout vk_sprite_pipeline_layout;

	void CreateSpritePipeline();
	void DestroySpritePipeline();
}