#pragma once

inline VkFence vk_fence_main;
inline VkSemaphore vk_semaphore_rendering;
inline VkSemaphore vk_semaphore_aquireimage;

void CreateSyncObjects();
void DestroySyncObjects();