#include "pch.h"
#include "vk_draw.h"
#include "vulkan.h"
#include "pipelines/vk_sprite_pipeline.h"
#include "renderpasses/vk_objects_pass.h"
#include "window/window.h"
#include "game/player.h"
#include "pipelines/vk_tiles_pipeline.h"
#include <graphics/vulkan/descriptor_sets/vk_atlas_descriptor_set.h>
#include "system_objects/vk_commandbuffers.h"
#include "game_objects/vk_vertexbuffer.h"
#include "game_objects/vk_objects_instancebuffer.h"
#include "system_objects/vk_queuefamilies.h"
#include "system_objects/vk_swapchain.h"
#include "game_objects/vk_renderimage.h"
#include "system_objects/vk_device.h"
#include "system_objects/vk_sync.h"
#include "system_objects/vk_queues.h"
#include "descriptor_sets/vk_level_descriptor_set.h"

static void DrawTiles()
{
	// Set pipeline
	vkCmdBindPipeline(vk_commandbuffer_main, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_tiles_pipeline);

	VkDescriptorSet sets[] = { vk_atlas_set , vk_level_set };
	vkCmdBindDescriptorSets(vk_commandbuffer_main, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_tiles_pipeline_layout, 0, _countof(sets), sets, 0, nullptr);

	VkDeviceSize offsets[] = { 0, 0 };
	VkBuffer vertexBuffers[] = { vk_quad_vertexbuffer };
	vkCmdBindVertexBuffers2(vk_commandbuffer_main, 0, _countof(vertexBuffers), vertexBuffers, offsets, nullptr, nullptr);

	vkCmdDraw(vk_commandbuffer_main, 6, 1, 0, 0);
}

static void DrawObjects()
{
	// Set pipeline
	vkCmdBindPipeline(vk_commandbuffer_main, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_sprite_pipeline);

	vkCmdBindDescriptorSets(vk_commandbuffer_main, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_sprite_pipeline_layout, 0, 1, &vk_atlas_set, 0, nullptr);

	ObjectData *const &objectData = (ObjectData *)vk_objects_instancebuffer_memory.map;

	objectData[0] = {
		.positionX = g_player.positionX,
		.positionY = g_player.positionY,
		.spriteIndex = 24,
	};

	VkDeviceSize offsets[] = { 0, 0 };
	VkBuffer vertexBuffers[] = { vk_quad_vertexbuffer, vk_objects_instancebuffer };
	vkCmdBindVertexBuffers2(vk_commandbuffer_main, 0, _countof(vertexBuffers), vertexBuffers, offsets, nullptr, nullptr);

	vkCmdDraw(vk_commandbuffer_main, 6, 1, 0, 0);
}

static void BlitImage(int swapchainImageIndex)
{
	VkImageMemoryBarrier2 toTransferDstBarrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
		.srcStageMask = VK_PIPELINE_STAGE_2_NONE,
		.srcAccessMask = VK_ACCESS_2_NONE,
		.dstStageMask = VK_PIPELINE_STAGE_2_BLIT_BIT,
		.dstAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		.srcQueueFamilyIndex = vk_queue_family_indices.mainQueueFamily,
		.dstQueueFamilyIndex = vk_queue_family_indices.mainQueueFamily,
		.image = vk_swapchain_images[swapchainImageIndex],
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.levelCount = 1,
			.layerCount = 1,
		},
	};

	VkDependencyInfo toTransferDstDependency{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &toTransferDstBarrier
	};

	vkCmdPipelineBarrier2(vk_commandbuffer_main, &toTransferDstDependency);

	int xSize = (int)SCREEN_WIDTH * vk_render_scale;
	int ySize = (int)SCREEN_HEIGHT * vk_render_scale;
	int xOffset = (vk_swapchain_width - xSize) / 2;
	int yOffset = (vk_swapchain_height - ySize) / 2;
	VkImageBlit2 region{
		.sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2,
		.srcSubresource = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.mipLevel = 0,
			.layerCount = 1,
		},
		.srcOffsets = {
			{0, 0, 0},
			{SCREEN_WIDTH, SCREEN_HEIGHT, 1},
		},
		.dstSubresource = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.mipLevel = 0,
			.layerCount = 1,
		},
		.dstOffsets = {
			{xOffset, yOffset, 0},
			{xOffset + xSize, yOffset + ySize, 1},
		},
	};

	VkBlitImageInfo2 blitInfo{
		.sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2,
		.srcImage = vk_render_image,
		.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		.dstImage = vk_swapchain_images[swapchainImageIndex],
		.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		.regionCount = 1,
		.pRegions = &region,
		.filter = VK_FILTER_NEAREST,
	};

	vkCmdBlitImage2(vk_commandbuffer_main, &blitInfo);

	VkImageMemoryBarrier2 toPresentSrcBarrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
		.srcStageMask = VK_PIPELINE_STAGE_2_BLIT_BIT,
		.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_2_NONE,
		.dstAccessMask = VK_ACCESS_2_NONE,
		.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		.srcQueueFamilyIndex = vk_queue_family_indices.mainQueueFamily,
		.dstQueueFamilyIndex = vk_queue_family_indices.mainQueueFamily,
		.image = vk_swapchain_images[swapchainImageIndex],
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.levelCount = 1,
			.layerCount = 1,
		},
	};

	VkDependencyInfo toPresentSrcDependency{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &toPresentSrcBarrier
	};

	vkCmdPipelineBarrier2(vk_commandbuffer_main, &toPresentSrcDependency);
}

void DrawFrame(int swapchainImageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VkAssert(vkResetCommandPool(vk_device, vk_commandpool_main, 0));
	VkAssert(vkBeginCommandBuffer(vk_commandbuffer_main, &beginInfo));

	// Set viewport
	VkViewport viewport{
		.x = 0, .y = (float)SCREEN_HEIGHT,
		.width = (float)SCREEN_WIDTH,
		.height = -(float)SCREEN_HEIGHT,
		.minDepth = 0,
		.maxDepth = 1,
	};
	vkCmdSetViewport(vk_commandbuffer_main, 0, 1, &viewport);

	VkRect2D scissor{
		.offset = { 0, 0 },
		.extent = {
			.width = SCREEN_WIDTH,
			.height = SCREEN_HEIGHT
		},
	};
	vkCmdSetScissor(vk_commandbuffer_main, 0, 1, &scissor);

	VkClearValue clear{
		.color = {.float32 = {0, 0, 0, 1}}
	};

	VkRenderPassBeginInfo begin{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = vk_objects_pass,
		.framebuffer = vk_render_framebuffer,
		.renderArea = {
			.extent = {.width = SCREEN_WIDTH, .height = SCREEN_HEIGHT},
		},
		.clearValueCount = 1,
		.pClearValues = &clear,
	};

	VkSubpassBeginInfo subBegin{
		.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO,
		.contents = VK_SUBPASS_CONTENTS_INLINE,
	};

	vkCmdBeginRenderPass2(vk_commandbuffer_main, &begin, &subBegin);

	// Set push constants
	int32_t pushData[] = { LEVEL_WIDTH, LEVEL_HEIGHT, TILE_SIZE };
	vkCmdPushConstants(vk_commandbuffer_main, vk_tiles_pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pushData), &pushData);

	DrawTiles();
	DrawObjects();

	VkSubpassEndInfo subEnd{
		.sType = VK_STRUCTURE_TYPE_SUBPASS_END_INFO,
	};

	vkCmdEndRenderPass2(vk_commandbuffer_main, &subEnd);

	BlitImage(swapchainImageIndex);

	VkAssert(vkEndCommandBuffer(vk_commandbuffer_main));

	VkCommandBufferSubmitInfo commandBufferSubmitInfo{};
	commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
	commandBufferSubmitInfo.commandBuffer = vk_commandbuffer_main;

	const VkSemaphoreSubmitInfo semaphoreSubmitInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
		.semaphore = vk_semaphore_rendering,
	};

	const VkSemaphoreSubmitInfo waitInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
		.semaphore = vk_semaphore_acquireimage,
	};

	VkSubmitInfo2 submitInfo{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
		.waitSemaphoreInfoCount = 1,
		.pWaitSemaphoreInfos = &waitInfo,
		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &commandBufferSubmitInfo,
		.signalSemaphoreInfoCount = 1,
		.pSignalSemaphoreInfos = &semaphoreSubmitInfo,
	};
	VkAssert(vkQueueSubmit2(vk_queue_main, 1, &submitInfo, vk_fence_main));
}