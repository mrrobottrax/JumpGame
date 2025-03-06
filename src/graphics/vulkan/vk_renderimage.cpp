#include "pch.h"
#include "vk_renderimage.h"
#include "vk_device.h"
#include "vulkan.h"
#include "vk_queuefamilies.h"
#include "renderpasses/vk_objects_pass.h"
#include "vk_memory.h"
#include "window/window.h"

void CreateRenderImage()
{
	constexpr VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;

	VkImageCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = format,
		.extent = {
			.width = SCREEN_WIDTH,
			.height = SCREEN_HEIGHT,
			.depth = 1,
		},
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 1,
		.pQueueFamilyIndices = &vk_queue_family_indices.mainQueueFamily,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
	};

	VkAssert(vkCreateImage(vk_device, &createInfo, nullptr, &vk_render_image));

	VkMemoryAllocateInfo allocateInfo{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = SCREEN_WIDTH * SCREEN_HEIGHT * 4 * 4,
		.memoryTypeIndex = vk_memory_types.local,
	};

	VkAssert(vkAllocateMemory(vk_device, &allocateInfo, nullptr, &vk_render_image_memory));

	VkBindImageMemoryInfo bindInfo{
		.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO,
		.image = vk_render_image,
		.memory = vk_render_image_memory,
		.memoryOffset = 0,
	};

	VkAssert(vkBindImageMemory2(vk_device, 1, &bindInfo));

	VkImageViewCreateInfo viewInfo{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = vk_render_image,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = format,
		.components = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY},
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.levelCount = 1,
			.layerCount = 1,
		},
	};

	VkAssert(vkCreateImageView(vk_device, &viewInfo, nullptr, &vk_render_image_view));

	VkFramebufferCreateInfo framebufferInfo{
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.renderPass = vk_objects_pass,
		.attachmentCount = 1,
		.pAttachments = &vk_render_image_view,
		.width = SCREEN_WIDTH,
		.height = SCREEN_HEIGHT,
		.layers = 1,
	};

	VkAssert(vkCreateFramebuffer(vk_device, &framebufferInfo, nullptr, &vk_render_framebuffer));
}

void DestroyRenderImage()
{
	vkFreeMemory(vk_device, vk_render_image_memory, nullptr);
	vkDestroyImageView(vk_device, vk_render_image_view, nullptr);
	vkDestroyFramebuffer(vk_device, vk_render_framebuffer, nullptr);
	vkDestroyImage(vk_device, vk_render_image, nullptr);
}
