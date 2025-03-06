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
#include "vk_vertexbuffer.h"
#include "vk_memory.h"
#include "vk_pipeline.h"
#include "renderpasses/vk_renderpasses.h"
#include "vk_draw.h"
#include "vk_renderimage.h"

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
	CreateRenderPasses();
	CreateSwapchain();
	CreateVertexBuffer();
	CreatePipeline();
	CreateRenderImage();
}

void EndVulkan()
{
	vkQueueWaitIdle(vk_queue_main);

	DestroyRenderImage();
	DestroyPipeline();
	DestroyVertexBuffer();
	DestroyRenderPasses();
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

	DrawFrame(imageIndex);

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