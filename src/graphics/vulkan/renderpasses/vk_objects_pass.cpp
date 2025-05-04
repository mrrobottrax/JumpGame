#include "pch.h"
#include "vk_objects_pass.h"
#include <graphics/vulkan/system_objects/vk_swapchain.h>
#include <graphics/vulkan/system_objects/vk_device.h>

namespace Graphics::Vulkan
{
	void CreateObjectsPass()
	{
		VkAttachmentDescription2 attachment{
			.sType = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2,
			.format = vk_swapchain_format,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		};

		VkAttachmentReference2 attachmentRef{
			.sType = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2,
			.attachment = 0,
			.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		};

		VkSubpassDescription2 subpass{
			.sType = VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2,
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.colorAttachmentCount = 1,
			.pColorAttachments = &attachmentRef
		};

		VkRenderPassCreateInfo2 createInfo{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2,
			.attachmentCount = 1,
			.pAttachments = &attachment,
			.subpassCount = 1,
			.pSubpasses = &subpass,
		};

		vkCreateRenderPass2(vk_device, &createInfo, nullptr, &vk_objects_pass);
	}

	void DestroyObjectsPass()
	{
		vkDestroyRenderPass(vk_device, vk_objects_pass, nullptr);
	}
}