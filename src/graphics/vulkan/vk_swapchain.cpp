#include "pch.h"
#include "vk_swapchain.h"
#include "vk_device.h"
#include "vk_surface.h"
#include "vk_physicaldevice.h"

void CreateSwapchain()
{
	uint32_t imageCount;
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physicaldevice, vk_surface, &capabilities);

	imageCount = capabilities.minImageCount + 1;
	if (imageCount > capabilities.maxImageCount) imageCount = capabilities.maxImageCount;

	uint32_t width = capabilities.currentExtent.width;
	uint32_t height = capabilities.currentExtent.height;

	VkSwapchainCreateInfoKHR createInfo{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = vk_surface,
		.minImageCount = imageCount,
		.imageFormat = VK_FORMAT_R8G8B8A8_UNORM,
		.imageExtent = {.width = width, .height = height},
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	};

	vkCreateSwapchainKHR(vk_device, &createInfo, nullptr, &vk_swapchain);
}

void DestroySwapchain()
{
	vkDestroySwapchainKHR(vk_device, vk_swapchain, nullptr);
}
