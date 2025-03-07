#include "pch.h"
#include "vk_unform_buffer.h"
#include "vk_device.h"
#include "vk_queuefamilies.h"
#include "vulkan.h"
#include "vk_memory.h"

void CreateUniformBuffer()
{
	VkBufferCreateInfo bufferInfo{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = sizeof(ObjectData),
		.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 1,
		.pQueueFamilyIndices = &vk_queue_family_indices.mainQueueFamily,
	};

	VkAssert(vkCreateBuffer(vk_device, &bufferInfo, nullptr, &vk_uniform_buffer));

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(vk_device, vk_uniform_buffer, &requirements);

	size_t size = sizeof(ObjectData);
	if (size < requirements.size) size = requirements.size;

	VkMemoryAllocateInfo allocateInfo{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = size,
		.memoryTypeIndex = vk_memory_types.local_hostvisible,
	};

	VkResult(vkAllocateMemory(vk_device, &allocateInfo, nullptr, &vk_uniform_buffer_memory));

	VkBindBufferMemoryInfo bindInfo{
		.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO,
		.buffer = vk_uniform_buffer,
		.memory = vk_uniform_buffer_memory,
		.memoryOffset = 0,
	};

	VkAssert(vkBindBufferMemory2(vk_device, 1, &bindInfo));

	VkAssert(vkMapMemory(vk_device, vk_uniform_buffer_memory, 0, size, 0, (void **)&vk_uniform_buffer_map));
}

void DestroyUniformBuffer()
{
	vkDestroyBuffer(vk_device, vk_uniform_buffer, nullptr);
	vkFreeMemory(vk_device, vk_uniform_buffer_memory, nullptr);
}
