#include "pch.h"
#include "vk_objects_instancebuffer.h"
#include <graphics/vulkan/vulkan.h>
#include <graphics/vulkan/system_objects/vk_device.h>
#include <graphics/vulkan/system_objects/vk_queuefamilies.h>

void CreateObjectsBuffer()
{
	VkBufferCreateInfo bufferInfo{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = vk_objects_instancebuffer_size,
		.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 1,
		.pQueueFamilyIndices = &vk_queue_family_indices.mainQueueFamily,
	};

	VkAssert(vkCreateBuffer(vk_device, &bufferInfo, nullptr, &vk_objects_instancebuffer));
}

void DestroyObjectsBuffer()
{
	vkDestroyBuffer(vk_device, vk_objects_instancebuffer, nullptr);
}
