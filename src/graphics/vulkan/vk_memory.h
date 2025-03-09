#pragma once

struct MemoryTypes
{
	uint32_t local;
	uint32_t local_hostvisible;
};
inline MemoryTypes vk_memory_types;

struct MemoryAllocation
{
	VkDeviceMemory memory;
	VkDeviceSize offset;
	void *map;
};

inline VkDeviceMemory vk_static_host_memory;
inline void *vk_static_host_memory_map;

void GetMemoryTypes();
void AllocateStaticMemory();
void FreeStaticMemory();