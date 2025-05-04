#include "pch.h"
#include "vk_level_texture.h"
#include <window/window.h>
#include <graphics/vulkan/system_objects/vk_queuefamilies.h>
#include <graphics/vulkan/vulkan.h>
#include <graphics/vulkan/system_objects/vk_device.h>
#include <graphics/vulkan/system_objects/vk_commandbuffers.h>
#include <graphics/vulkan/system_objects/vk_queues.h>
#include <game/level.h>
#include <graphics/vulkan/vk_image_util.h>

namespace Graphics::Vulkan
{
	void create_level_image()
	{
		VkImageCreateInfo imageInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_R16_UINT,
			.extent = {.width = Window::LEVEL_WIDTH, .height = Window::LEVEL_HEIGHT, .depth = 1},
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_LINEAR,
			.usage = VK_IMAGE_USAGE_SAMPLED_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = &vk_queue_family_indices.mainQueueFamily,
			.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED,
		};

		vk_assert(vkCreateImage(vk_device, &imageInfo, nullptr, &vk_level_image));
	}

	void create_level_image_view()
	{
		// Create image view
		VkImageViewCreateInfo viewInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = vk_level_image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = VK_FORMAT_R16_UINT,
			.components = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY},
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.levelCount = 1,
				.layerCount = 1,
			},
		};

		vk_assert(vkCreateImageView(vk_device, &viewInfo, nullptr, &vk_level_view));

		VkImageSubresource subResource{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.mipLevel = 0,
			.arrayLayer = 0,
		};
		VkSubresourceLayout layout;
		vkGetImageSubresourceLayout(vk_device, vk_level_image, &subResource, &layout);

		// Copy data
		for (uint32_t row = 0; row < Window::LEVEL_HEIGHT; ++row)
		{
			memcpy(
				(char *)vk_level_memory.map + row * layout.rowPitch + layout.offset,
				LEVEL_DATA + row * Window::LEVEL_WIDTH,
				(size_t)Window::LEVEL_WIDTH * 2
			);
		}

		transition_image(VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, vk_level_image);
	}

	void destroy_level_image()
	{
		vkDestroyImageView(vk_device, vk_level_view, nullptr);
		vkDestroyImage(vk_device, vk_level_image, nullptr);
	}
}