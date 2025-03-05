#pragma once

inline VkDeviceMemory vk_tri_vertexbuffer_memory;
inline VkBuffer vk_tri_vertexbuffer;

void CreateVertexBuffer();
void DestroyVertexBuffer();