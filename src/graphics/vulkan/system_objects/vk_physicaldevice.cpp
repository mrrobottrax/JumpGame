#include "pch.h"
#include "vk_physicaldevice.h"
#include "vk_instance.h"
#include "console/console.h"

using namespace Console;

namespace Graphics::Vulkan
{
	struct DeviceEntry
	{
		VkPhysicalDevice vk_physicaldevice;
		int score;
	};

	void pick_physical_device()
	{
		uint32_t physicalDeviceCount;
		vkEnumeratePhysicalDevices(vk_instance, &physicalDeviceCount, nullptr);

		VkPhysicalDevice *devices = new VkPhysicalDevice[physicalDeviceCount];
		vkEnumeratePhysicalDevices(vk_instance, &physicalDeviceCount, devices);

		assert(physicalDeviceCount > 0);

		DeviceEntry bestDevice;

		log("DEVICES:");
		for (uint32_t i = 0; i < physicalDeviceCount; ++i)
		{
#pragma warning (push)
#pragma warning (disable:6385)
			VkPhysicalDevice &vk_physicaldevice = devices[i];
#pragma warning (pop)

			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(vk_physicaldevice, &properties);

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

			log("%s : %i", properties.deviceName, score);

			if (i == 0 || score > bestDevice.score)
			{
				bestDevice = {
					.vk_physicaldevice = vk_physicaldevice,
					.score = score,
				};
			}
		}

		vk_physicaldevice = bestDevice.vk_physicaldevice;

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(bestDevice.vk_physicaldevice, &properties);

		log("USING DEVICE: %s", properties.deviceName);

		delete[] devices;
	}
}