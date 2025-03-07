#pragma once

inline VkDeviceMemory vk_quad_vertexbuffer_memory;
inline VkBuffer vk_quad_vertexbuffer;

void CreateVertexBuffer();
void DestroyVertexBuffer();