#include "pch.h"
#include "vk_objects_instancebuffer.h"
#include "vk_device.h"
#include "vulkan.h"
#include "vk_memory.h"
#include "vk_queuefamilies.h"

void CreateObjectsBuffer()
{
	VkMemoryAllocateInfo allocateInfo{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = vk_objects_instancebuffer_size,
		.memoryTypeIndex = vk_memory_types.local_hostvisible,
	};

	VkAssert(vkAllocateMemory(vk_device, &allocateInfo, nullptr, &vk_objects_instancebuffer_memory));

	VkBufferCreateInfo bufferInfo{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = vk_objects_instancebuffer_size,
		.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 1,
		.pQueueFamilyIndices = &vk_queue_family_indices.mainQueueFamily,
	};

	VkAssert(vkCreateBuffer(vk_device, &bufferInfo, nullptr, &vk_objects_instancebuffer));

	VkBindBufferMemoryInfo bindInfo{
		.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO,
		.buffer = vk_objects_instancebuffer,
		.memory = vk_objects_instancebuffer_memory,
		.memoryOffset = 0,
	};

	VkAssert(vkBindBufferMemory2(vk_device, 1, &bindInfo));

	VkAssert(vkMapMemory(vk_device, vk_objects_instancebuffer_memory, 0, vk_objects_instancebuffer_size, 0, (void **)&vk_objects_instancebuffer_map));
}

void DestroyObjectsBuffer()
{
	vkFreeMemory(vk_device, vk_objects_instancebuffer_memory, nullptr);
	vkDestroyBuffer(vk_device, vk_objects_instancebuffer, nullptr);
}
