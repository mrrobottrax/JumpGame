#include "pch.h"
#include "vk_physicaldevice.h"
#include "vk_instance.h"
#include "console/console.h"

#include <cassert>
VkPhysicalDevice device;

struct DeviceEntry
{
	VkPhysicalDevice device;
	int score;
};

void PickPhysicalDevice()
{
	uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(vk_instance, &physicalDeviceCount, nullptr);

	VkPhysicalDevice *devices = new VkPhysicalDevice[physicalDeviceCount];
	vkEnumeratePhysicalDevices(vk_instance, &physicalDeviceCount, devices);

	assert(physicalDeviceCount > 0);

	DeviceEntry bestDevice;

	Log("DEVICES:");
	for (uint32_t i = 0; i < physicalDeviceCount; ++i)
	{
#pragma warning (push)
#pragma warning (disable:6385)
		VkPhysicalDevice &device = devices[i];
#pragma warning (pop)

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);

		int score = 0;
		switch (properties.deviceType)
		{
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				score += 10;
				break;

			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				score += 5;
				break;

			default:
				break;
		}

		Log("%s : %i", properties.deviceName, score);

		if (i == 0 || score > bestDevice.score)
		{
			bestDevice = {
				.device = device,
				.score = score,
			};
		}
	}

	device = bestDevice.device;

	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(bestDevice.device, &properties);

	Log("USING DEVICE: %s", properties.deviceName);

	delete[] devices;
}