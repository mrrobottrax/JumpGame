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
}

void EndVulkan()
{
	vkQueueWaitIdle(vk_queue_main);

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

	VkAssert(vkEndCommandBuffer(vk_commandbuffer_main));

	VkCommandBufferSubmitInfo commandBufferSubmitInfo{};
	commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
	commandBufferSubmitInfo.commandBuffer = vk_commandbuffer_main;

	VkSubmitInfo2 submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
	submitInfo.commandBufferInfoCount = 1;
	submitInfo.pCommandBufferInfos = &commandBufferSubmitInfo;
	VkAssert(vkQueueSubmit2(vk_queue_main, 1, &submitInfo, vk_fence_main));
}

void VkAssert(VkResult result)
{
	if (result < 0)
	{
		throw VulkanException("Vulkan Error:", result);
	}
}