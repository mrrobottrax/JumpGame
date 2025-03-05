#include "pch.h"
#include "vk_device.h"
#include "vk_physicaldevice.h"
#include "vulkan.h"
#include "console/console.h"
#include "vk_queuefamilies.h"

void CreateDevice()
{
	float queuePriority = 1;

	VkDeviceQueueCreateInfo queueInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = vk_queuefamilyindices.mainQueueFamily,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority,
	};

	VkDeviceCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &queueInfo,
	};

	VkAssert(vkCreateDevice(vk_physicaldevice, &createInfo, nullptr, &vk_device));
}

void DestroyDevice()
{
	vkDestroyDevice(vk_device, nullptr);
}