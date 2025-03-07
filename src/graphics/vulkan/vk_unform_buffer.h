#pragma once

inline VkBuffer vk_uniform_buffer;
inline VkDeviceMemory vk_uniform_buffer_memory;

struct ObjectData
{
	float positionX, positionY, positionZ;
	uint32_t spriteIndex;
};

inline ObjectData *vk_uniform_buffer_map;

void CreateUniformBuffer();
void DestroyUniformBuffer();