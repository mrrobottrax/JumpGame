#include "pch.h"
#include "vk_device.h"
#include "vk_physicaldevice.h"
#include "vulkan.h"
#include "console/console.h"

VkDevice vk_device;

void CreateDevice()
{
	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties2(vk_physicaldevice, &queueFamilyCount, nullptr);

	VkQueueFamilyProperties *queueFamilies = new VkQueueFamilyProperties[queueFamilyCount];
	vkGetPhysicalDeviceQueueFamilyProperties(vk_physicaldevice, &queueFamilyCount, queueFamilies);

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
			Log("  -Graphics");

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

	/*VkDeviceQueueCreateInfo queueInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = 0,
		.queueCount = 1,
	};

	VkDeviceCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &queueInfo,
	};

	VkAssert(vkCreateDevice(vk_physicaldevice, &createInfo, nullptr, &vk_device));*/

	delete[] queueFamilies;
}

void DestroyDevice()
{
	//vkDestroyDevice(vk_device, nullptr);
}