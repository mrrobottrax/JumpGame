#pragma once

struct MemoryTypes
{
	uint32_t local;
	uint32_t local_hostvisible;
};
inline MemoryTypes vk_memory_types;

struct MemoryAllocation
{
	VkMappedMemoryRange range;
	VkDeviceMemory memory;
	VkDeviceSize offset;
	void *map;
};

inline VkDeviceMemory vk_static_host_memory;
inline void *vk_static_host_memory_map;

inline VkDeviceMemory vk_static_local_memory;

void GetMemoryTypes();
void AllocateStaticMemory();
void FreeStaticMemory();