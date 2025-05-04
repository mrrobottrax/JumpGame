#include "pch.h"
#include "vk_vertexbuffer.h"
#include <graphics/vulkan/system_objects/vk_queuefamilies.h>
#include <graphics/vulkan/vulkan.h>
#include <graphics/vulkan/system_objects/vk_device.h>
#include <graphics/vulkan/vk_memory.h>

namespace Graphics::Vulkan
{
	constexpr float quad_verts[] = {
		0, 0,
		1, 0,
		0, 1,

		1, 0,
		1, 1,
		0, 1,
	};

	void create_vertex_buffer()
	{
		VkBufferCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = sizeof(quad_verts),
			.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = &vk_queue_family_indices.mainQueueFamily,
		};

		vk_assert(vkCreateBuffer(vk_device, &createInfo, nullptr, &vk_quad_vertexbuffer));
	}

	void load_vertex_buffer()
	{
		memcpy(vk_quad_vertexbuffer_memory.map, quad_verts, sizeof(quad_verts));
	}

	void destroy_vertex_buffer()
	{
		vkDestroyBuffer(vk_device, vk_quad_vertexbuffer, nullptr);
	}
}