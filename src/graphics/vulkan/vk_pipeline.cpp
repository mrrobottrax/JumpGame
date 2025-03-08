#include "pch.h"
#include "vk_pipeline.h"
#include "vk_device.h"
#include "vulkan.h"
#include "file/file.h"
#include "renderpasses/vk_objects_pass.h"

class ShaderModuleWrapper
{
	VkShaderModule module;

public:
	ShaderModuleWrapper(VkShaderModule module) : module(module)
	{}

	~ShaderModuleWrapper()
	{
		vkDestroyShaderModule(vk_device, module, nullptr);
	}

	operator VkShaderModule &() { return module; }
};

static ShaderModuleWrapper CreateShaderModule(const wchar_t name[]);

void CreatePipeline()
{
	// Create layout

	VkPushConstantRange pushConstantRange{
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		.offset = 0,
		.size = 8,
	};

	VkPipelineLayoutCreateInfo layoutInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 0,
		.pSetLayouts = nullptr,
		.pushConstantRangeCount = 1,
		.pPushConstantRanges = &pushConstantRange,
	};

	VkAssert(vkCreatePipelineLayout(vk_device, &layoutInfo, nullptr, &vk_pipeline_layout));

	// Shader stages

	ShaderModuleWrapper vertexModule = CreateShaderModule(L"tri.vert.spv");
	ShaderModuleWrapper fragmentModule = CreateShaderModule(L"tri.frag.spv");

	VkPipelineShaderStageCreateInfo fragmentStage{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = fragmentModule,
		.pName = "main",
	};

	VkPipelineShaderStageCreateInfo vertexStage{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = vertexModule,
		.pName = "main",
	};

	VkPipelineShaderStageCreateInfo stages[] = { fragmentStage, vertexStage };

	// Create pipeline state

	VkVertexInputBindingDescription vertexBindingDescription{
		.binding = 0,
		.stride = 8,
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};

	VkVertexInputBindingDescription instanceBindingDescription{
		.binding = 1,
		.stride = 16,
		.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE,
	};

	VkVertexInputBindingDescription bindings[] = { vertexBindingDescription, instanceBindingDescription };

	VkVertexInputAttributeDescription vertexAttributeDescription{
		.location = 0,
		.binding = 0,
		.format = VK_FORMAT_R32G32_SFLOAT,
		.offset = 0,
	};

	VkVertexInputAttributeDescription offsetAttributeDescription{
		.location = 1,
		.binding = 1,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = 0,
	};


	VkVertexInputAttributeDescription spriteAttributeDescription{
		.location = 2,
		.binding = 1,
		.format = VK_FORMAT_R32_UINT,
		.offset = 12,
	};

	VkVertexInputAttributeDescription attributes[] = { vertexAttributeDescription, offsetAttributeDescription, spriteAttributeDescription };

	VkPipelineVertexInputStateCreateInfo vertexState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = _countof(bindings),
		.pVertexBindingDescriptions = bindings,
		.vertexAttributeDescriptionCount = _countof(attributes),
		.pVertexAttributeDescriptions = attributes,
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
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
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
			.blendEnable = VK_FALSE,
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		},
	};

	VkPipelineColorBlendStateCreateInfo colorBlendState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.attachmentCount = _countof(attachments),
		.pAttachments = attachments,
		.blendConstants = {1, 1, 1, 1},
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
		.renderPass = vk_objects_pass,
	};

	VkAssert(vkCreateGraphicsPipelines(vk_device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &vk_pipeline));
}

void DestroyPipeline()
{
	vkDestroyPipelineLayout(vk_device, vk_pipeline_layout, nullptr);
	vkDestroyPipeline(vk_device, vk_pipeline, nullptr);
}

ShaderModuleWrapper CreateShaderModule(const wchar_t name[])
{
	FileHandle file = LoadEntireFile(name);

	VkShaderModule module;
	VkShaderModuleCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = file.Size(),
		.pCode = (uint32_t *)file.Data(),
	};

	VkAssert(vkCreateShaderModule(vk_device, &createInfo, nullptr, &module));

	return ShaderModuleWrapper(module);
}
