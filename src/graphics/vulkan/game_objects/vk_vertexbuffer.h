#pragma once

#include <graphics/vulkan/vk_memory.h>

namespace Graphics::Vulkan
{
	inline VkBuffer vk_quad_vertexbuffer;

	inline MemoryAllocation vk_quad_vertexbuffer_memory;

	void create_vertex_buffer();
	void load_vertex_buffer();
	void destroy_vertex_buffer();
}