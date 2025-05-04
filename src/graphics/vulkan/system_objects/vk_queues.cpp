#include "pch.h"
#include "vk_queues.h"
#include "vk_queuefamilies.h"
#include "vk_device.h"

namespace Graphics::Vulkan
{
	void get_device_queues()
	{
		VkDeviceQueueInfo2 queueInfo{
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
			.queueFamilyIndex = vk_queue_family_indices.mainQueueFamily,
			.queueIndex = 0
		};
		vkGetDeviceQueue2(vk_device, &queueInfo, &vk_queue_main);
	}
}
