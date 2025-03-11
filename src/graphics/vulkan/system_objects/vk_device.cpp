#include "pch.h"
#include "vk_device.h"
#include "vk_physicaldevice.h"
#include "console/console.h"
#include "vk_queuefamilies.h"
#include <graphics/vulkan/vulkan.h>
#include "graphics/vulkan/extensions/vk_ext_memorypriority.h"

constexpr const char *REQUIRED_EXTENSIONS[] = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

void CreateDevice()
{
	uint32_t propertyCount;
	vkEnumerateDeviceExtensionProperties(vk_physicaldevice, nullptr, &propertyCount, nullptr);

	VkExtensionProperties *extensions = new VkExtensionProperties[propertyCount];
	vkEnumerateDeviceExtensionProperties(vk_physicaldevice, nullptr, &propertyCount, extensions);

	std::vector<const char *> usedExts(_countof(REQUIRED_EXTENSIONS));
	for (int i = 0; i < _countof(REQUIRED_EXTENSIONS); ++i)
	{
		usedExts[i] = REQUIRED_EXTENSIONS[i];
	}

	Log("DEVICE EXTENSIONS:");
	for (uint32_t i = 0; i < propertyCount; ++i)
	{
	#pragma warning (push)
	#pragma warning (disable:6385)
		VkExtensionProperties &properties = extensions[i];
	#pragma warning (pop)

		Log(properties.extensionName);

		if (strcmp(properties.extensionName, "VK_EXT_pageable_device_local_memory") == 0)
		{
			usedExts.push_back("VK_EXT_memory_priority");
			usedExts.push_back("VK_EXT_pageable_device_local_memory");

			vk_device_extensions.memory_priority = true;
		}
	}

	delete[] extensions;

	float queuePriority = 1;

	VkDeviceQueueCreateInfo queueInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = vk_queue_family_indices.mainQueueFamily,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority,
	};

	VkPhysicalDeviceSynchronization2Features sync2{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES,
		.synchronization2 = VK_TRUE
	};

	VkPhysicalDeviceFeatures2 features{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext = &sync2,
		.features = {}
	};

	VkDeviceCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = &features,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &queueInfo,
		.enabledExtensionCount = (uint32_t)usedExts.size(),
		.ppEnabledExtensionNames = &usedExts[0],
	};

	VkAssert(vkCreateDevice(vk_physicaldevice, &createInfo, nullptr, &vk_device));

	if (vk_device_extensions.memory_priority)
	{
		GetMemoryPriorityFunctionPointers();
	}
}

void DestroyDevice()
{
	vkDestroyDevice(vk_device, nullptr);
}