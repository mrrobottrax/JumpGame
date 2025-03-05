#pragma once

inline VkFence vk_fence_main;
inline VkSemaphore vk_semaphore_rendering;

void CreateSyncObjects();
void DestroySyncObjects();