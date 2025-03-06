#include "pch.h"
#include "vk_vertexbuffer.h"
#include "vk_device.h"
#include "vk_queuefamilies.h"
#include "vulkan.h"
#include "vk_memory.h"

constexpr float tri_verts[] = {
	-0.5f, -0.5f, 0.5f,
	-0.5f,  0.5f, 0.5f,
	 0.5f, -0.5f, 0.5f,

	 0.5f, -0.5f, 0.5f,
	-0.5f,  0.5f, 0.5f,
	 0.5f,  0.5f, 0.5f,
};

void CreateVertexBuffer()
{
	VkBufferCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = sizeof(tri_verts),
		.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 1,
		.pQueueFamilyIndices = &vk_queue_family_indices.mainQueueFamily,
	};

	VkAssert(vkCreateBuffer(vk_device, &createInfo, nullptr, &vk_tri_vertexbuffer));

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(vk_device, vk_tri_vertexbuffer, &requirements);

	size_t size = sizeof(tri_verts);
	if (size < requirements.size) size = requirements.size;

	VkMemoryAllocateInfo memInfo{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = size,
		.memoryTypeIndex = vk_memory_types.local_hostvisible,
	};

	VkAssert(vkAllocateMemory(vk_device, &memInfo, nullptr, &vk_tri_vertexbuffer_memory));

	VkBindBufferMemoryInfo bindInfo{
		.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO,
		.buffer = vk_tri_vertexbuffer,
		.memory = vk_tri_vertexbuffer_memory,
		.memoryOffset = 0,
	};
	VkAssert(vkBindBufferMemory2(vk_device, 1, &bindInfo));

	void *pData;
	VkAssert(vkMapMemory(vk_device, vk_tri_vertexbuffer_memory, 0, size, 0, &pData));
	memcpy(pData, tri_verts, sizeof(tri_verts));
}

void DestroyVertexBuffer()
{
	vkFreeMemory(vk_device, vk_tri_vertexbuffer_memory, nullptr);
	vkDestroyBuffer(vk_device, vk_tri_vertexbuffer, nullptr);
}
