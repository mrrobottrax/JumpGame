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
#include "game_objects/vk_win_screen.h"

using namespace Console;

namespace Graphics::Vulkan
{
	void get_memory_types()
	{
		VkPhysicalDeviceMemoryProperties memory;
		vkGetPhysicalDeviceMemoryProperties(vk_physicaldevice, &memory);

		vk_memory_types = {
			.local = UINT32_MAX,
			.hostvisible = UINT32_MAX,
		};

		log("MEMORY TYPES:");
		for (uint32_t i = 0; i < memory.memoryTypeCount; ++i)
		{
			VkMemoryType &mem = memory.memoryTypes[i];

			log("%i: ", i);
			log("  Heap: ", mem.heapIndex);

			if (mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
				log("  -Device local");

			if (mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
				log("  -Host visible");

			if (mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
				log("  -Host coherent");

			if (mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
				log("  -Host cached");

			if (mem.propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
				log("  -Lazily allocated");

			if (mem.propertyFlags & VK_MEMORY_PROPERTY_PROTECTED_BIT)
				log("  -Protected");

			if (mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD)
				log("  -Device coherent AMD");

			if (mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD)
				log("  -Device uncached AMD");

			if (mem.propertyFlags & VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV)
				log("  -RDMA capable NV");

			if (mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT &&
				!(mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
				!(mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) &&
				!(mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD))
			{
				if (vk_memory_types.local == -1)
				{
					log("  !local");
					vk_memory_types.local = i;
				}
			}

			if (mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT &&
				!(mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) &&
				!(mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD))
			{
				if (vk_memory_types.hostvisible == -1)
				{
					log("  !hostvisible");
					vk_memory_types.hostvisible = i;
				}
			}

			if (mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT &&
				mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT &&
				!(mem.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) &&
				!(mem.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD))
			{
				if (!vk_memory_types.hostvisible_is_local)
				{
					log("  !hostvisible_local");
					vk_memory_types.hostvisible = i;
					vk_memory_types.hostvisible_is_local = true;
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

		void bind_memory()
		{
			switch (type)
			{
				case MemoryEntry::Image:
					vk_assert(vkBindImageMemory(vk_device, *pImage, allocation.memory, allocation.offset));
					break;
				case MemoryEntry::Buffer:
					vk_assert(vkBindBufferMemory(vk_device, *pBuffer, allocation.memory, allocation.offset));
					break;
				default:
					break;
			}
		}
	};

	template<size_t size>
	static VkDeviceSize allocate_memory(MemoryEntry(&entries)[size], VkDeviceMemory &memory, uint32_t memoryType, float priority)
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

		VkMemoryAllocateInfo allocateInfo
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize = offset,
			.memoryTypeIndex = memoryType
		};

		vk_assert(vkAllocateMemory(vk_device, &allocateInfo, nullptr, &memory));
		if (vk_device_extensions.memory_priority)
		{
			vkSetDeviceMemoryPriorityEXT(vk_device, memory, priority);
		}

		offset = 0;
		for (int i = 0; i < _countof(entries); ++i)
		{
			offset = (VkDeviceSize)ceilf((float)offset / entries[i].requirements.alignment) * entries[i].requirements.alignment;

			entries[i].allocation.range.memory = memory;
			entries[i].allocation.memory = memory;
			entries[i].bind_memory();

			offset += entries[i].requirements.size;
		}

		return offset;
	}

	static void allocate_host_visible_memory()
	{
		MemoryEntry entries[] = {
			{vk_win_memory, vk_win_image},
			{vk_level_memory, vk_level_image},
			{vk_atlas_memory, vk_atlas_image},
			{vk_objects_instancebuffer_memory, vk_objects_instancebuffer},
			{vk_quad_vertexbuffer_memory, vk_quad_vertexbuffer},
		};

		VkDeviceSize size = allocate_memory(entries, vk_static_host_memory, vk_memory_types.hostvisible, 1);

		vk_assert(vkMapMemory(vk_device, vk_static_host_memory, 0, size, 0, &vk_static_host_memory_map));

		VkDeviceSize offset = 0;
		for (int i = 0; i < _countof(entries); ++i)
		{
			offset = (VkDeviceSize)ceilf((float)offset / entries[i].requirements.alignment) * entries[i].requirements.alignment;

			entries[i].allocation.map = (char *)vk_static_host_memory_map + offset;

			offset += entries[i].requirements.size;
		}
	}

	static void allocate_device_local_memory()
	{
		MemoryEntry entries[] = {
			{vk_render_image_memory, vk_render_image},
		};

		allocate_memory(entries, vk_static_local_memory, vk_memory_types.local, 0);
	}

	void allocate_static_memory()
	{
		allocate_host_visible_memory();
		allocate_device_local_memory();
	}

	void free_static_memory()
	{
		vkFreeMemory(vk_device, vk_static_local_memory, nullptr);
		vkFreeMemory(vk_device, vk_static_host_memory, nullptr);
	}
}