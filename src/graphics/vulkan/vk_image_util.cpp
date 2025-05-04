#include "pch.h"
#include "vk_image_util.h"
#include "vulkan.h"
#include "system_objects/vk_device.h"
#include "system_objects/vk_commandbuffers.h"
#include "system_objects/vk_queuefamilies.h"
#include "system_objects/vk_queues.h"

namespace Graphics::Vulkan
{
	void transition_image(VkImageLayout oldLayout, VkImageLayout newLayout, VkImage image)
	{
		vk_assert(vkResetCommandPool(vk_device, vk_commandpool_main, 0));
		VkCommandBufferBeginInfo beginInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
		};
		vk_assert(vkBeginCommandBuffer(vk_commandbuffer_main, &beginInfo));

		VkImageMemoryBarrier2 imageBarrier{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
			.srcStageMask = VK_PIPELINE_STAGE_2_NONE,
			.srcAccessMask = VK_ACCESS_NONE,
			.dstStageMask = VK_PIPELINE_STAGE_2_NONE,
			.dstAccessMask = VK_ACCESS_NONE,
			.oldLayout = oldLayout,
			.newLayout = newLayout,
			.srcQueueFamilyIndex = vk_queue_family_indices.mainQueueFamily,
			.dstQueueFamilyIndex = vk_queue_family_indices.mainQueueFamily,
			.image = image,
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

		vk_assert(vkEndCommandBuffer(vk_commandbuffer_main));

		VkCommandBufferSubmitInfo bufferSubmitInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.commandBuffer = vk_commandbuffer_main,
		};

		VkSubmitInfo2 submit{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &bufferSubmitInfo,
		};
		vk_assert(vkQueueSubmit2(vk_queue_main, 1, &submit, VK_NULL_HANDLE));
		vk_assert(vkQueueWaitIdle(vk_queue_main));
	}
}