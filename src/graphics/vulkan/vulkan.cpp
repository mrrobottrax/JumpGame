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

void InitVulkan()
{
	CreateInstance();
	PickPhysicalDevice();
	GetQueueFamilies();
	CreateDevice();
	GetDeviceQueues();
	CreateSurface();
	CreateCommandBuffers();
}

void EndVulkan()
{
	DestroyCommandBuffers();
	DestroySurface();
	DestroyDevice();
	DestroyInstance();
}

void RenderFrameVulkan()
{
	/*VkCommandBufferBeginInfo beginInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};

	vkResetCommandPool(vk_device, vk_commandpool_main, 0);
	vkBeginCommandBuffer(vk_commandbuffer_main, &beginInfo);

	vkEndCommandBuffer(vk_commandbuffer_main);

	VkCommandBufferSubmitInfo commandBufferSubmitInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
		.commandBuffer = vk_commandbuffer_main
	};

	VkSubmitInfo2 submitInfo{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &commandBufferSubmitInfo
	};
	vkQueueSubmit2(vk_queue_main, 1, &submitInfo, VK_NULL_HANDLE);*/
}

void VkAssert(VkResult result)
{
	if (result < 0)
	{
		throw VulkanException("Vulkan Error:", result);
	}
}