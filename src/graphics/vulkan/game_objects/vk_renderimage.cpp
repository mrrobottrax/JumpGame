#include "pch.h"
#include "vk_renderimage.h"
#include "window/window.h"
#include <graphics/vulkan/system_objects/vk_queuefamilies.h>
#include <graphics/vulkan/vulkan.h>
#include <graphics/vulkan/system_objects/vk_device.h>
#include <graphics/vulkan/renderpasses/vk_objects_pass.h>
#include <graphics/vulkan/vk_memory.h>

namespace Graphics::Vulkan
{
	constexpr VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;

	void create_render_image()
	{
		VkImageCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = format,
			.extent = {
				.width = Window::SCREEN_WIDTH,
				.height = Window::SCREEN_HEIGHT,
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

		vk_assert(vkCreateImage(vk_device, &createInfo, nullptr, &vk_render_image));
	}

	void create_render_image_view()
	{
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

		vk_assert(vkCreateImageView(vk_device, &viewInfo, nullptr, &vk_render_image_view));

		VkFramebufferCreateInfo framebufferInfo{
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.renderPass = vk_objects_pass,
			.attachmentCount = 1,
			.pAttachments = &vk_render_image_view,
			.width = Window::SCREEN_WIDTH,
			.height = Window::SCREEN_HEIGHT,
			.layers = 1,
		};

		vk_assert(vkCreateFramebuffer(vk_device, &framebufferInfo, nullptr, &vk_render_framebuffer));
	}

	void destroy_render_image()
	{
		vkDestroyImageView(vk_device, vk_render_image_view, nullptr);
		vkDestroyFramebuffer(vk_device, vk_render_framebuffer, nullptr);
		vkDestroyImage(vk_device, vk_render_image, nullptr);
	}
}
