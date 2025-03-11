#include "pch.h"
#include "vk_memory.h"
#include "console/console.h"
#include <graphics/vulkan/game_objects/vk_atlas_texture.h>
#include <graphics/vulkan/vulkan.h>
#include "system_objects/vk_physicaldevice.h"
#include "system_objects/vk_device.h"
#include "game_objects/vk_objects_instancebuffer.h"
#include "game_objects/vk_vertexbuffer.h"
#include "game_objects/vk_renderimage.h"
#include "game_objects/vk_level_texture.h"

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

struct MemoryEntry
{
	enum EntryType
	{
		Image,
		Buffer
	} type;

	VkMemoryRequirements requirements;
	MemoryAllocation &allocation;

	union
	{
		VkImage *pImage;
		VkBuffer *pBuffer;
	};

	MemoryEntry(MemoryAllocation &allocation, VkImage &image) : allocation(allocation), pImage(&image), type(Image)
	{
		vkGetImageMemoryRequirements(vk_device, image, &requirements);
	}

	MemoryEntry(MemoryAllocation &allocation, VkBuffer &buffer) : allocation(allocation), pBuffer(&buffer), type(Buffer)
	{
		vkGetBufferMemoryRequirements(vk_device, buffer, &requirements);
	}

	void BindMemory()
	{
		switch (type)
		{
			case MemoryEntry::Image:
				VkAssert(vkBindImageMemory(vk_device, *pImage, allocation.memory, allocation.offset));
				break;
			case MemoryEntry::Buffer:
				VkAssert(vkBindBufferMemory(vk_device, *pBuffer, allocation.memory, allocation.offset));
				break;
			default:
				break;
		}
	}
};

template<size_t size>
static VkDeviceSize AllocateMemory(MemoryEntry(&entries)[size], VkDeviceMemory &memory, uint32_t memoryType)
{
	VkDeviceSize offset = 0;
	for (int i = 0; i < _countof(entries); ++i)
	{
		offset = (VkDeviceSize)ceilf((float)offset / entries[i].requirements.alignment) * entries[i].requirements.alignment;

		entries[i].allocation.range.offset = offset;
		entries[i].allocation.offset = offset;
		entries[i].allocation.range.size = entries[i].requirements.size;
		entries[i].allocation.range.pNext = nullptr;
		entries[i].allocation.range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		offset += entries[i].requirements.size;
	}

	constexpr VkDeviceSize minAlloc = 262144;
	VkDeviceSize allocationSize = offset;

	if (allocationSize < minAlloc) allocationSize = minAlloc;

	VkMemoryAllocateInfo allocateInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = allocationSize,
		.memoryTypeIndex = memoryType
	};

	VkAssert(vkAllocateMemory(vk_device, &allocateInfo, nullptr, &memory));

	offset = 0;
	for (int i = 0; i < _countof(entries); ++i)
	{
		offset = (VkDeviceSize)ceilf((float)offset / entries[i].requirements.alignment) * entries[i].requirements.alignment;

		entries[i].allocation.range.memory = memory;
		entries[i].allocation.memory = memory;
		entries[i].BindMemory();

		offset += entries[i].requirements.size;
	}

	return offset;
}

static void AllocateHostVisibleMemory()
{
	MemoryEntry entries[] = {
		{vk_atlas_memory, vk_atlas_image},
		{vk_objects_instancebuffer_memory, vk_objects_instancebuffer},
		{vk_quad_vertexbuffer_memory, vk_quad_vertexbuffer},
		{vk_level_memory, vk_level_image},
	};

	VkDeviceSize size = AllocateMemory(entries, vk_static_host_memory, vk_memory_types.local_hostvisible);

	VkAssert(vkMapMemory(vk_device, vk_static_host_memory, 0, size, 0, &vk_static_host_memory_map));

	VkDeviceSize offset = 0;
	for (int i = 0; i < _countof(entries); ++i)
	{
		offset = (VkDeviceSize)ceilf((float)offset / entries[i].requirements.alignment) * entries[i].requirements.alignment;

		entries[i].allocation.map = (char *)vk_static_host_memory_map + offset;

		offset += entries[i].requirements.size;
	}
}

static void AllocateDeviceLocalMemory()
{
	MemoryEntry entries[] = {
		{vk_render_image_memory, vk_render_image},
	};

	AllocateMemory(entries, vk_static_local_memory, vk_memory_types.local);
}

void AllocateStaticMemory()
{
	AllocateHostVisibleMemory();
	AllocateDeviceLocalMemory();
}

void FreeStaticMemory()
{
	vkFreeMemory(vk_device, vk_static_local_memory, nullptr);
	vkFreeMemory(vk_device, vk_static_host_memory, nullptr);
}
