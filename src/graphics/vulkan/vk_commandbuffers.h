#pragma once

inline VkCommandPool vk_commandpool_main;
inline VkCommandBuffer vk_commandbuffer_main;

void CreateCommandBuffers();
void DestroyCommandBuffers();