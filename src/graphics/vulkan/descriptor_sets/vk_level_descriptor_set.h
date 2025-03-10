#pragma once

inline VkDescriptorSet vk_level_set;
inline VkDescriptorSetLayout vk_level_set_layout;

void CreateLevelDescriptorSet();
void DestroyLevelDescriptorSet();