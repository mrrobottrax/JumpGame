#pragma once
#include <graphics/vulkan/vk_memory.h>
#include "graphics/vulkan/vk_spritedata.h"

namespace Graphics::Vulkan
{
	inline constexpr size_t vk_objects_instancebuffer_size = 1024;
	inline constexpr size_t vk_objects_instancebuffer_maxobjects = vk_objects_instancebuffer_size / sizeof(SpriteData);

	inline VkBuffer vk_objects_instancebuffer;
	inline MemoryAllocation vk_objects_instancebuffer_memory;

	void CreateObjectsBuffer();
	void DestroyObjectsBuffer();
}