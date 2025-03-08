#pragma once

inline VkDescriptorSet vk_tiles_set;
inline VkDescriptorPool vk_tiles_set_pool;
inline VkDescriptorSetLayout vk_tiles_set_layout;

void CreateTilesSet();
void DestroyTilesSet();