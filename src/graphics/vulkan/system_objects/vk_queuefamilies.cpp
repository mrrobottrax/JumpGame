#include "pch.h"
#include "vk_queuefamilies.h"
#include "vk_physicaldevice.h"
#include <console/console.h>

using namespace Console;

namespace Graphics::Vulkan
{
	void get_queue_families()
	{
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(vk_physicaldevice, &queueFamilyCount, nullptr);

		VkQueueFamilyProperties *queueFamilies = new VkQueueFamilyProperties[queueFamilyCount];
		vkGetPhysicalDeviceQueueFamilyProperties(vk_physicaldevice, &queueFamilyCount, queueFamilies);

		vk_queue_family_indices.mainQueueFamily = -1;

		log("Queues:");
		for (uint32_t i = 0; i < queueFamilyCount; ++i)
		{
#pragma warning (push)
#pragma warning (disable:6385)
			VkQueueFamilyProperties &properties = queueFamilies[i];
#pragma warning (pop)

			log("%i:", i);
			log("  count: %i:", properties.queueCount);

			if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				log("  -Graphics");

				if (vk_queue_family_indices.mainQueueFamily == -1)
				{
					vk_queue_family_indices.mainQueueFamily = i;
				}
			}

			if (properties.queueFlags & VK_QUEUE_COMPUTE_BIT)
				log("  -Compute");

			if (properties.queueFlags & VK_QUEUE_TRANSFER_BIT)
				log("  -Transfer");

			if (properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
				log("  -Sparse binding");

			if (properties.queueFlags & VK_QUEUE_PROTECTED_BIT)
				log("  -Protected");

			if (properties.queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR)
				log("  -Video decode");

			if (properties.queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR)
				log("  -Video encode");

			if (properties.queueFlags & VK_QUEUE_OPTICAL_FLOW_BIT_NV)
				log("  -VK_QUEUE_OPTICAL_FLOW_BIT_NV");
		}

		delete[] queueFamilies;

		log("PICKED QUEUES:");
		log("Main Queue: %i", vk_queue_family_indices.mainQueueFamily);
	}
}