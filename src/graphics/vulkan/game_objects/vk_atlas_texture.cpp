#include "pch.h"
#include "vk_atlas_texture.h"
#include <graphics/vulkan/system_objects/vk_queuefamilies.h>
#include <graphics/vulkan/vulkan.h>
#include <graphics/vulkan/system_objects/vk_device.h>
#include <graphics/vulkan/vk_image_util.h>

namespace Graphics::Vulkan
{
	static UncompressedImage atlasImage;

	void create_atlas_texture()
	{
		atlasImage = (UncompressedImage &&)LoadAndUncompressPNG(L"data/tilemap.png");

		VkImageCreateInfo imageInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_R8G8B8A8_SRGB,
			.extent = {.width = atlasImage.width, .height = atlasImage.height, .depth = 1},
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

		vk_assert(vkCreateImage(vk_device, &imageInfo, nullptr, &vk_atlas_image));
	}

	void load_atlas_texture()
	{
		// Create image view
		VkImageViewCreateInfo viewInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = vk_atlas_image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = VK_FORMAT_R8G8B8A8_SRGB,
			.components = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY},
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.levelCount = 1,
				.layerCount = 1,
			},
		};

		vk_assert(vkCreateImageView(vk_device, &viewInfo, nullptr, &vk_atlas_view));

		VkImageSubresource subResource{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.mipLevel = 0,
			.arrayLayer = 0,
		};
		VkSubresourceLayout layout;
		vkGetImageSubresourceLayout(vk_device, vk_atlas_image, &subResource, &layout);

		// Copy data
		for (uint32_t row = 0; row < atlasImage.height; ++row)
		{
			memcpy(
				(char *)vk_atlas_memory.map + row * layout.rowPitch + layout.offset,
				atlasImage.pData + row * atlasImage.width * 4,
				(size_t)atlasImage.width * 4
			);
		}

		atlasImage.~UncompressedImage();

		transition_image(VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, vk_atlas_image);
	}

	void destroy_atlas_texture()
	{
		vkDestroyImage(vk_device, vk_atlas_image, nullptr);
		vkDestroyImageView(vk_device, vk_atlas_view, nullptr);
	}
}