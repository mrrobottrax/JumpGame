#pragma once

namespace Graphics::Vulkan
{
	inline VkFormat vk_swapchain_format;
	inline VkColorSpaceKHR vk_swapchain_colourspace;
	inline uint32_t vk_swapchain_width, vk_swapchain_height;
	inline int vk_render_scale;

	inline uint32_t vk_swapchain_image_count;
	inline VkSwapchainKHR vk_swapchain;
	inline VkImage *vk_swapchain_images;
	inline VkImageView *vk_swapchain_image_views;

	void create_swapchain();
	void destroy_swapchain();
	void get_swapchain_format();
	void recreate_swapchain();
}