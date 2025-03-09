#pragma once

inline VkDescriptorSet vk_atlas_set;
inline VkDescriptorPool vk_atlas_set_pool;
inline VkDescriptorSetLayout vk_atlas_set_layout;

void CreateAtlasDescriptorSet();
void DestroyAtlasDescriptorSet();