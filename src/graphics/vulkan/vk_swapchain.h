#pragma once

inline VkFormat vk_swapchain_format;
inline uint32_t vk_width, vk_height;

inline uint32_t vk_swapchain_image_count;
inline VkSwapchainKHR vk_swapchain;
inline VkImage *vk_swapchain_images;
inline VkImageView *vk_swapchain_image_views;
inline VkFramebuffer *vk_swapchain_framebuffers;

void CreateSwapchain();
void DestroySwapchain();
void GetSwapchainFormat();