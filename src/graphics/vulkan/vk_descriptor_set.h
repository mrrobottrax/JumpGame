#pragma once

inline VkDescriptorSetLayout vk_set_layout;
inline VkDescriptorSet vk_descriptor_set;
inline VkDescriptorPool vk_descriptor_pool;

void CreateDescriptorSet();
void DestroyDescriptorSet();