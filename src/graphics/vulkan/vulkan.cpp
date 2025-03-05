#include "pch.h"
#include "exceptions/exceptions.h"
#include "vulkan.h"
#include "vk_instance.h"
#include "vk_physicaldevice.h"
#include "vk_device.h"
#include "vk_debug.h"
#include "vk_surface.h"
#include "vk_queuefamilies.h"
#include "vk_queues.h"
#include "vk_commandbuffers.h"
#include "vk_sync.h"
#include "vk_swapchain.h"

void InitVulkan()
{
	CreateInstance();
	PickPhysicalDevice();
	GetQueueFamilies();
	CreateDevice();
	GetDeviceQueues();
	CreateSurface();
	CreateCommandBuffers();
	CreateSyncObjects();
	CreateSwapchain();
}

void EndVulkan()
{
	vkQueueWaitIdle(vk_queue_main);

	DestroySwapchain();
	DestroySyncObjects();
	DestroyCommandBuffers();
	DestroySurface();
	DestroyDevice();
	DestroyInstance();
}

void RenderFrameVulkan()
{
	if (vkGetFenceStatus(vk_device, vk_fence_main) != VK_SUCCESS) return;
	vkResetFences(vk_device, 1, &vk_fence_main);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VkAssert(vkResetCommandPool(vk_device, vk_commandpool_main, 0));
	VkAssert(vkBeginCommandBuffer(vk_commandbuffer_main, &beginInfo));

	/*VkClearValue clear{
		.color = {.float32 = {0, 0.5f, 1, 1}}
	};

	VkRenderPassBeginInfo begin{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.clearValueCount = 1,
		.pClearValues = &clear,
		.renderPass = vk_renderpass_main,
		.framebuffer =
	};

	VkSubpassBeginInfo subBegin{
		.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO,
		.contents = VK_SUBPASS_CONTENTS_INLINE,
	};

	vkCmdBeginRenderPass2(vk_commandbuffer_main, &begin, &subBegin);

	VkSubpassEndInfo subEnd{
		.sType = VK_STRUCTURE_TYPE_SUBPASS_END_INFO,
	};

	vkCmdEndRenderPass2(vk_commandbuffer_main, &subEnd);*/

	VkAssert(vkEndCommandBuffer(vk_commandbuffer_main));

	/*VkCommandBufferSubmitInfo commandBufferSubmitInfo{};
	commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
	commandBufferSubmitInfo.commandBuffer = vk_commandbuffer_main;

	const VkSemaphoreSubmitInfo semaphoreSubmitInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
		.semaphore = vk_semaphore_rendering,
		.value = 1,
	};

	VkSubmitInfo2 submitInfo{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &commandBufferSubmitInfo,
		.signalSemaphoreInfoCount = 1,
		.pSignalSemaphoreInfos = &semaphoreSubmitInfo,
	};
	VkAssert(vkQueueSubmit2(vk_queue_main, 1, &submitInfo, vk_fence_main));

	VkPresentInfoKHR present{
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &vk_semaphore_rendering,
		.swapchainCount = 1,
		.pSwapchains = &vk_swapchain,
	};

	vkQueuePresentKHR(vk_queue_main, &present);*/
}

void VkAssert(VkResult result)
{
	if (result < 0)
	{
		throw VulkanException("Vulkan Error:", result);
	}
}