#include "pch.h"
#include "vk_device.h"
#include "vk_physicaldevice.h"
#include "vulkan.h"
#include "console/console.h"
#include "vk_queuefamilies.h"

constexpr const char *REQUIRED_EXTENSIONS[] = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

void CreateDevice()
{
	uint32_t propertyCount;
	vkEnumerateDeviceExtensionProperties(vk_physicaldevice, nullptr, &propertyCount, nullptr);

	VkExtensionProperties *extensions = new VkExtensionProperties[propertyCount];
	vkEnumerateDeviceExtensionProperties(vk_physicaldevice, nullptr, &propertyCount, extensions);

	Log("DEVICE EXTENSIONS:");
	for (uint32_t i = 0; i < propertyCount; ++i)
	{
	#pragma warning (push)
	#pragma warning (disable:6385)
		VkExtensionProperties &properties = extensions[i];
	#pragma warning (pop)

		Log(properties.extensionName);
	}

	delete[] extensions;

	float queuePriority = 1;

	VkDeviceQueueCreateInfo queueInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = vk_queuefamilyindices.mainQueueFamily,
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
		.enabledExtensionCount = _countof(REQUIRED_EXTENSIONS),
		.ppEnabledExtensionNames = REQUIRED_EXTENSIONS
	};

	VkAssert(vkCreateDevice(vk_physicaldevice, &createInfo, nullptr, &vk_device));
}

void DestroyDevice()
{
	vkDestroyDevice(vk_device, nullptr);
}