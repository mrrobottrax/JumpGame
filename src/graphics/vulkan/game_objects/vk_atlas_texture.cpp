#include "pch.h"
#include "vk_atlas_texture.h"
#include <console/console.h>
#include <graphics/vulkan/system_objects/vk_queuefamilies.h>
#include <graphics/vulkan/vulkan.h>
#include <graphics/vulkan/system_objects/vk_device.h>
#include <graphics/vulkan/system_objects/vk_commandbuffers.h>
#include <graphics/vulkan/system_objects/vk_queues.h>

static UncompressedImage atlasImage;

void CreateAtlasTexture()
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

	VkAssert(vkCreateImage(vk_device, &imageInfo, nullptr, &vk_atlas_image));
}

void LoadAtlasTexture()
{
	// Copy data
	const uint32_t size = atlasImage.width * atlasImage.height * 4;

	memcpy(vk_atlas_memory.map, atlasImage.pData, size);
	atlasImage.~UncompressedImage();

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

	VkAssert(vkCreateImageView(vk_device, &viewInfo, nullptr, &vk_atlas_view));

	// Transition layout
	VkCommandBufferBeginInfo beginInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};
	VkAssert(vkBeginCommandBuffer(vk_commandbuffer_main, &beginInfo));

	VkImageMemoryBarrier2 imageBarrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
		.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
		.srcAccessMask = VK_ACCESS_NONE,
		.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
		.dstAccessMask = VK_ACCESS_NONE,
		.oldLayout = VK_IMAGE_LAYOUT_PREINITIALIZED,
		.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		.srcQueueFamilyIndex = vk_queue_family_indices.mainQueueFamily,
		.dstQueueFamilyIndex = vk_queue_family_indices.mainQueueFamily,
		.image = vk_atlas_image,
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.levelCount = 1,
			.layerCount = 1,
		},
	};

	VkDependencyInfo dependency{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &imageBarrier,
	};
	vkCmdPipelineBarrier2(vk_commandbuffer_main, &dependency);

	VkAssert(vkEndCommandBuffer(vk_commandbuffer_main));

	VkCommandBufferSubmitInfo bufferSubmitInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
		.commandBuffer = vk_commandbuffer_main,
	};

	VkSubmitInfo2 submit{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &bufferSubmitInfo,
	};
	VkAssert(vkQueueSubmit2(vk_queue_main, 1, &submit, VK_NULL_HANDLE));
	VkAssert(vkQueueWaitIdle(vk_queue_main));
}

void DestroyAtlasTexture()
{
	vkDestroyImage(vk_device, vk_atlas_image, nullptr);
	vkDestroyImageView(vk_device, vk_atlas_view, nullptr);
}
