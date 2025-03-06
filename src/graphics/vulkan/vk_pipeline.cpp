#include "pch.h"
#include "vk_pipeline.h"
#include "vk_device.h"
#include "vulkan.h"
#include "vk_render_pass.h"

void CreatePipeline()
{
	/*VkPipelineLayoutCreateInfo layoutInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
	};

	VkAssert(vkCreatePipelineLayout(vk_device, &layoutInfo, nullptr, &vk_pipeline_layout));

	VkPipelineShaderStageCreateInfo fragmentStage{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.pName = "frag",
	};

	VkPipelineShaderStageCreateInfo vertexStage{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.pName = "vert",
	};

	VkPipelineShaderStageCreateInfo stages[] = { fragmentStage, vertexStage };

	VkPipelineVertexInputStateCreateInfo vertexState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
	};

	VkPipelineInputAssemblyStateCreateInfo inputState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	};

	VkPipelineTessellationStateCreateInfo tessellationState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
	};

	VkPipelineViewportStateCreateInfo viewportState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
	};

	VkPipelineRasterizationStateCreateInfo rasterizationState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
	};

	VkGraphicsPipelineCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.flags = VK_DYNAMIC_STATE_VIEWPORT | VK_DYNAMIC_STATE_SCISSOR,
		.stageCount = _countof(stages),
		.pStages = stages,
		.pVertexInputState = &vertexState,
		.pInputAssemblyState = &inputState,
		.pTessellationState = &tessellationState,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizationState,
		.layout = vk_pipeline_layout,
		.renderPass = vk_render_pass,
	};

	VkAssert(vkCreateGraphicsPipelines(vk_device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &vk_pipeline));*/
}

void DestroyPipeline()
{}
