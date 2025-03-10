#pragma once
#include <graphics/vulkan/vk_memory.h>

inline constexpr size_t vk_objects_instancebuffer_size = 1024;

inline VkBuffer vk_objects_instancebuffer;
inline MemoryAllocation vk_objects_instancebuffer_memory;

struct ObjectData
{
	float positionX, positionY;
	uint32_t spriteIndex;
};

inline constexpr size_t vk_objects_instancebuffer_maxobjects = vk_objects_instancebuffer_size / sizeof(ObjectData);

void CreateObjectsBuffer();
void DestroyObjectsBuffer();