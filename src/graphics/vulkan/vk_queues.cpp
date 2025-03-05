#include "pch.h"
#include "vk_queues.h"
#include "vk_queuefamilies.h"
#include "vk_device.h"

void GetDeviceQueues()
{
	VkDeviceQueueInfo2 queueInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
		.queueFamilyIndex = vk_queuefamilyindices.mainQueueFamily,
		.queueIndex = 0
	};
	vkGetDeviceQueue2(vk_device, &queueInfo, &vk_queue_main);
}
