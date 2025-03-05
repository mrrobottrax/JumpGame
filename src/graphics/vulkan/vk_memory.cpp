#include "pch.h"
#include "vk_memory.h"
#include "vk_physicaldevice.h"
#include "console/console.h"

void GetMemoryTypes()
{
	VkPhysicalDeviceMemoryProperties memory;
	vkGetPhysicalDeviceMemoryProperties(vk_physicaldevice, &memory);

	vk_memory_types = {
		.local = UINT32_MAX,
		.local_hostvisible = UINT32_MAX,
	};

	Log("MEMORY TYPES:");
	for (uint32_t i = 0; i < memory.memoryTypeCount; ++i)
	{
		VkMemoryType &mem = memory.memoryTypes[i];

		Log("%i: ", i);
		Log("  Heap: ", mem.heapIndex);

		if (mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
			Log("  -Device local");

		if (mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			Log("  -Host visible");

		if (mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
			Log("  -Host coherent");

		if (mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
			Log("  -Host cached");

		if (mem.propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
			Log("  -Lazily allocated");

		if (mem.propertyFlags & VK_MEMORY_PROPERTY_PROTECTED_BIT)
			Log("  -Protected");

		if (mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD)
			Log("  -Device coherent AMD");

		if (mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD)
			Log("  -Device uncached AMD");

		if (mem.propertyFlags & VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV)
			Log("  -RDMA capable NV");

		if (mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT &&
			!(mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
			!(mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) &&
			!(mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD))
		{
			Log("  !local");
			if (vk_memory_types.local == -1)
			{
				vk_memory_types.local = i;
			}
		}

		if (mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT &&
			mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT &&
			mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT &&
			!(mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) &&
			!(mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD))
		{
			Log("  !local_hostvisible");
			if (vk_memory_types.local_hostvisible == -1)
			{
				vk_memory_types.local_hostvisible = i;
			}
		}
	}
}