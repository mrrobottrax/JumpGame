#pragma once

inline VkImage vk_atlas_image;
inline VkImageView vk_atlas_view;
inline VkDeviceMemory vk_atlas_memory;
inline VkSampler vk_atlas_sampler;
inline void *vk_atlas_map;

void CreateAtlas();
void DestroyAtlas();