#include "pch.h"
#include "vk_pipeline.h"
#include "vk_device.h"
#include "vulkan.h"
#include "vk_render_pass.h"

//static VkShaderModule CreateShaderModule(const char name[]);

void CreatePipeline()
{
	// Create layout
	/*VkPipelineLayoutCreateInfo layoutInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
	};

	VkAssert(vkCreatePipelineLayout(vk_device, &layoutInfo, nullptr, &vk_pipeline_layout));

	// Shader stages

	VkShaderModule vertexModule = CreateShaderModule();
	VkShaderModule fragmentModule = CreateShaderModule();

	VkPipelineShaderStageCreateInfo fragmentStage{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = fragmentModule,
		.pName = "frag",
	};

	VkPipelineShaderStageCreateInfo vertexStage{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = vertexModule,
		.pName = "vert",
	};

	VkPipelineShaderStageCreateInfo stages[] = { fragmentStage, vertexStage };

	// Create pipeline state

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
		.viewportCount = 1,
		.scissorCount = 1,
	};

	VkPipelineRasterizationStateCreateInfo rasterizationState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.lineWidth = 1,
	};

	VkPipelineMultisampleStateCreateInfo multisampleState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
	};

	VkPipelineDepthStencilStateCreateInfo depthStencilState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable = VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
		.depthBoundsTestEnable = VK_TRUE,
		.minDepthBounds = 0,
		.maxDepthBounds = 1,
	};

	VkPipelineColorBlendAttachmentState attachments[] = {
		{
			.blendEnable = VK_FALSE
		},
	};

	VkPipelineColorBlendStateCreateInfo colorBlendState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.attachmentCount = _countof(attachments),
		.pAttachments = attachments,
	};

	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};

	VkPipelineDynamicStateCreateInfo dynamicState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = 2,
		.pDynamicStates = dynamicStates,
	};

	// Create pipeline

	VkGraphicsPipelineCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = _countof(stages),
		.pStages = stages,
		.pVertexInputState = &vertexState,
		.pInputAssemblyState = &inputState,
		.pTessellationState = &tessellationState,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizationState,
		.pMultisampleState = &multisampleState,
		.pDepthStencilState = &depthStencilState,
		.pColorBlendState = &colorBlendState,
		.pDynamicState = &dynamicState,
		.layout = vk_pipeline_layout,
		.renderPass = vk_render_pass,
	};

	VkAssert(vkCreateGraphicsPipelines(vk_device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &vk_pipeline));*/
}

void DestroyPipeline()
{
	/*vkDestroyPipelineLayout(vk_device, vk_pipeline_layout, nullptr);
	vkDestroyPipeline(vk_device, vk_pipeline, nullptr);*/
}

//VkShaderModule CreateShaderModule(const char name[])
//{
//	FileHandle file = LoadEntireFile(name);
//
//	VkShaderModule module;
//	VkShaderModuleCreateInfo createInfo{
//		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
//		.codeSize = file.size,
//		.pCode = file.pData,
//	};
//
//	VkAssert(vkCreateShaderModule(vk_device, &createInfo, nullptr, &module));
//
//	return module;
//}
