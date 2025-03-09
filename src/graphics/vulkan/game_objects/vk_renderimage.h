#pragma once

inline VkImage vk_render_image;
inline VkImageView vk_render_image_view;
inline VkFramebuffer vk_render_framebuffer;
inline VkDeviceMemory vk_render_image_memory;

void CreateRenderImage();
void DestroyRenderImage();