#pragma once

inline constexpr size_t vk_objects_instancebuffer_size = 1024;

inline VkBuffer vk_objects_instancebuffer;
inline VkDeviceMemory vk_objects_instancebuffer_memory;

struct ObjectData
{
	float positionX, positionY, positionZ;
	uint32_t spriteIndex;
};

inline ObjectData *vk_objects_instancebuffer_map;
inline constexpr size_t vk_objects_instancebuffer_maxobjects = vk_objects_instancebuffer_size / sizeof(ObjectData);

void CreateObjectsBuffer();
void DestroyObjectsBuffer();