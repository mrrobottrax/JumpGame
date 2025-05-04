#include "pch.h"
#include "vk_queuefamilies.h"
#include "vk_physicaldevice.h"
#include <console/console.h>

using namespace Console;

namespace Graphics::Vulkan
{
	void GetQueueFamilies()
	{
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(vk_physicaldevice, &queueFamilyCount, nullptr);

		VkQueueFamilyProperties *queueFamilies = new VkQueueFamilyProperties[queueFamilyCount];
		vkGetPhysicalDeviceQueueFamilyProperties(vk_physicaldevice, &queueFamilyCount, queueFamilies);

		vk_queue_family_indices.mainQueueFamily = -1;

		Log("Queues:");
		for (uint32_t i = 0; i < queueFamilyCount; ++i)
		{
#pragma warning (push)
#pragma warning (disable:6385)
			VkQueueFamilyProperties &properties = queueFamilies[i];
#pragma warning (pop)

			Log("%i:", i);
			Log("  count: %i:", properties.queueCount);

			if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				Log("  -Graphics");

				if (vk_queue_family_indices.mainQueueFamily == -1)
				{
					vk_queue_family_indices.mainQueueFamily = i;
				}
			}

			if (properties.queueFlags & VK_QUEUE_COMPUTE_BIT)
				Log("  -Compute");

			if (properties.queueFlags & VK_QUEUE_TRANSFER_BIT)
				Log("  -Transfer");

			if (properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
				Log("  -Sparse binding");

			if (properties.queueFlags & VK_QUEUE_PROTECTED_BIT)
				Log("  -Protected");

			if (properties.queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR)
				Log("  -Video decode");

			if (properties.queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR)
				Log("  -Video encode");

			if (properties.queueFlags & VK_QUEUE_OPTICAL_FLOW_BIT_NV)
				Log("  -VK_QUEUE_OPTICAL_FLOW_BIT_NV");
		}

		delete[] queueFamilies;

		Log("PICKED QUEUES:");
		Log("Main Queue: %i", vk_queue_family_indices.mainQueueFamily);
	}
}