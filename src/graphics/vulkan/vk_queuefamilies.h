#pragma once

struct QueueFamilyIndices
{
	uint32_t mainQueueFamily;
};
inline QueueFamilyIndices vk_queuefamilyindices;

void GetQueueFamilies();