#pragma once

struct MemoryTypes
{
	uint32_t local;
	uint32_t local_hostvisible;
};
inline MemoryTypes vk_memory_types;

void GetMemoryTypes();