#include "pch.h"
#include "vk_commandbuffers.h"
#include "vk_queuefamilies.h"
#include "vk_device.h"

namespace Graphics::Vulkan
{
	void create_command_buffers()
	{
		VkCommandPoolCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.queueFamilyIndex = vk_queue_family_indices.mainQueueFamily,
		};

		vkCreateCommandPool(vk_device, &createInfo, nullptr, &vk_commandpool_main);

		VkCommandBufferAllocateInfo allocateInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = vk_commandpool_main,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1,
		};

		vkAllocateCommandBuffers(vk_device, &allocateInfo, &vk_commandbuffer_main);
	}

	void destroy_command_buffer()
	{
		vkDestroyCommandPool(vk_device, vk_commandpool_main, nullptr);
	}
}