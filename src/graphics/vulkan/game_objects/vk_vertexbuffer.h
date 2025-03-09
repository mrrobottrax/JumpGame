#pragma once

#include <graphics/vulkan/vk_memory.h>

inline VkBuffer vk_quad_vertexbuffer;

inline MemoryAllocation vk_quad_vertexbuffer_memory;

void CreateVertexBuffer();
void LoadVertexBuffer();
void DestroyVertexBuffer();