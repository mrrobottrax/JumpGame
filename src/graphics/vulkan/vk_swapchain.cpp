#include "pch.h"
#include "vk_swapchain.h"
#include "vk_device.h"
#include "vk_surface.h"
#include "vk_physicaldevice.h"
#include "vulkan.h"

static void CreateSwapchainObjects()
{
	VkSwapchainCreateInfoKHR createInfo{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = vk_surface,
		.minImageCount = vk_swapchain_image_count,
		.imageFormat = vk_swapchain_format,
		.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
		.imageExtent = {.width = vk_swapchain_width, .height = vk_swapchain_height},
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR,
		.clipped = VK_TRUE,
	};

	VkAssert(vkCreateSwapchainKHR(vk_device, &createInfo, nullptr, &vk_swapchain));

	VkAssert(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_swapchain_image_count, vk_swapchain_images));

	for (uint32_t i = 0; i < vk_swapchain_image_count; ++i)
	{
		VkImageViewCreateInfo viewInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = vk_swapchain_images[i],
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = createInfo.imageFormat,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.levelCount = 1,
				.layerCount = 1,
			},
		};

		VkAssert(vkCreateImageView(vk_device, &viewInfo, nullptr, &vk_swapchain_image_views[i]));

		//VkFramebufferCreateInfo framebufferInfo{
		//	.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		//	.renderPass = vk_objects_pass,
		//	.attachmentCount = 1,
		//	.pAttachments = &vk_swapchain_image_views[i],
		//	.width = vk_swapchain_width,
		//	.height = vk_swapchain_height,
		//	.layers = 1,
		//};

		//vkCreateFramebuffer(vk_device, &framebufferInfo, nullptr, &vk_swapchain_framebuffers[i]);
	}
}

static void DestroySwapchainObjects()
{
	for (uint32_t i = 0; i < vk_swapchain_image_count; ++i)
	{
		//vkDestroyFramebuffer(vk_device, vk_swapchain_framebuffers[i], nullptr);
		vkDestroyImageView(vk_device, vk_swapchain_image_views[i], nullptr);
	}

	vkDestroySwapchainKHR(vk_device, vk_swapchain, nullptr);
}

void CreateSwapchain()
{
	VkSurfaceCapabilitiesKHR capabilities;
	VkAssert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physicaldevice, vk_surface, &capabilities));

	vk_swapchain_width = capabilities.currentExtent.width;
	vk_swapchain_height = capabilities.currentExtent.height;

	vk_swapchain_image_count = capabilities.minImageCount + 1;
	if (vk_swapchain_image_count > capabilities.maxImageCount) vk_swapchain_image_count = capabilities.maxImageCount;

	vk_swapchain_images = new VkImage[vk_swapchain_image_count];
	//vk_swapchain_framebuffers = new VkFramebuffer[vk_swapchain_image_count];
	vk_swapchain_image_views = new VkImageView[vk_swapchain_image_count];

	CreateSwapchainObjects();
}

void DestroySwapchain()
{
	DestroySwapchainObjects();

	delete[] vk_swapchain_images;
	//delete[] vk_swapchain_framebuffers;
	delete[] vk_swapchain_image_views;
}

void GetSwapchainFormat()
{
	vk_swapchain_format = VK_FORMAT_R8G8B8A8_SRGB;
}

void RecreateSwapchain()
{
	VkSurfaceCapabilitiesKHR capabilities;
	VkAssert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physicaldevice, vk_surface, &capabilities));

	vk_swapchain_width = capabilities.currentExtent.width;
	vk_swapchain_height = capabilities.currentExtent.height;

	DestroySwapchainObjects();
	CreateSwapchainObjects();
}