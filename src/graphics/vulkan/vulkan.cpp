#include "pch.h"
#include "exceptions/exceptions.h"
#include "vulkan.h"
#include "vk_instance.h"
#include "vk_physicaldevice.h"
#include "vk_device.h"
#include "vk_surface.h"
#include "vk_queuefamilies.h"
#include "vk_queues.h"
#include "vk_commandbuffers.h"
#include "vk_sync.h"
#include "vk_swapchain.h"
#include "vk_render_pass.h"
#include "vk_vertexbuffer.h"
#include "vk_memory.h"
#include "vk_pipeline.h"

static void DrawSprite(float posX, float posY);

void InitVulkan()
{
	CreateInstance();
	PickPhysicalDevice();
	GetQueueFamilies();
	GetMemoryTypes();
	CreateDevice();
	GetDeviceQueues();
	CreateSurface();
	CreateCommandBuffers();
	CreateSyncObjects();
	GetSwapchainFormat();
	CreateRenderPass();
	CreateSwapchain();
	CreateVertexBuffer();
	CreatePipeline();
}

void EndVulkan()
{
	vkQueueWaitIdle(vk_queue_main);

	DestroyPipeline();
	DestroyVertexBuffer();
	DestroyRenderPass();
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

	uint32_t imageIndex = 0;
	VkResult result;
	while (true)
	{
		result = vkAcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, vk_semaphore_acquireimage, VK_NULL_HANDLE, &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapchain();
			continue;
		}

		break;
	}

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VkAssert(vkResetCommandPool(vk_device, vk_commandpool_main, 0));
	VkAssert(vkBeginCommandBuffer(vk_commandbuffer_main, &beginInfo));

	VkClearValue clear{
		.color = {.float32 = {0, 0.5f, 1, 1}}
	};

	VkRenderPassBeginInfo begin{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = vk_render_pass,
		.framebuffer = vk_swapchain_framebuffers[imageIndex],
		.renderArea = {
			.extent = {.width = vk_swapchain_width, .height = vk_swapchain_height},
		},
		.clearValueCount = 1,
		.pClearValues = &clear,
	};

	VkSubpassBeginInfo subBegin{
		.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO,
		.contents = VK_SUBPASS_CONTENTS_INLINE,
	};

	VkSubpassEndInfo subEnd{
	.sType = VK_STRUCTURE_TYPE_SUBPASS_END_INFO,
	};

	vkCmdBeginRenderPass2(vk_commandbuffer_main, &begin, &subBegin);

		// Set viewport
	VkViewport viewport{
		.x = 0, .y = 0,
		.width = (float)vk_swapchain_width,
		.height = (float)vk_swapchain_height,
		.minDepth = 0,
		.maxDepth = 1,
	};
	vkCmdSetViewport(vk_commandbuffer_main, 0, 1, &viewport);

	VkRect2D scissor{
		.offset = { 0, 0 },
		.extent = {
			.width = vk_swapchain_width,
			.height = vk_swapchain_height
		},
	};
	vkCmdSetScissor(vk_commandbuffer_main, 0, 1, &scissor);

	// Set shader
	vkCmdBindPipeline(vk_commandbuffer_main, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipeline);

	// Draw all objects
	DrawSprite(0, 0);

	vkCmdEndRenderPass2(vk_commandbuffer_main, &subEnd);

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

	VkPresentInfoKHR present{
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &vk_semaphore_rendering,
		.swapchainCount = 1,
		.pSwapchains = &vk_swapchain,
		.pImageIndices = &imageIndex,
	};

	result = vkQueuePresentKHR(vk_queue_main, &present);
	if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		VkAssert(vkWaitForFences(vk_device, 1, &vk_fence_main, VK_TRUE, UINT64_MAX));
		RecreateSwapchain();
	}
}

void VkAssert(VkResult result)
{
	if (result < 0)
	{
		throw VulkanException("Vulkan Error:", result);
	}
}

void DrawSprite(float posX, float posY)
{
	// Draw object
	float pushData[] = { posX, posY, 0 };
	vkCmdPushConstants(vk_commandbuffer_main, vk_pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, 12, &pushData);

	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers2(vk_commandbuffer_main, 0, 1, &vk_tri_vertexbuffer, &offset, nullptr, nullptr);

	vkCmdDraw(vk_commandbuffer_main, 6, 1, 0, 0);
}