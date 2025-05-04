#include "pch.h"
#include "vk_swapchain.h"
#include "vk_device.h"
#include "vk_surface.h"
#include "vk_physicaldevice.h"
#include "window/window.h"
#include <graphics/vulkan/vulkan.h>

namespace Graphics::Vulkan
{
	static void CalculateRenderScale()
	{
		int scaleX = vk_swapchain_width / SCREEN_WIDTH;
		int scaleY = vk_swapchain_height / SCREEN_HEIGHT;
		int minScale = scaleX;
		if (scaleY < minScale) minScale = scaleY;

		vk_render_scale = minScale;
	}

	static void CreateSwapchainObjects()
	{
		VkSwapchainCreateInfoKHR createInfo{
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = vk_surface,
			.minImageCount = vk_swapchain_image_count,
			.imageFormat = vk_swapchain_format,
			.imageColorSpace = vk_swapchain_colourspace,
			.imageExtent = {.width = vk_swapchain_width, .height = vk_swapchain_height},
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = VK_PRESENT_MODE_FIFO_KHR,
			.clipped = VK_TRUE,
		};

		VkAssert(vkCreateSwapchainKHR(vk_device, &createInfo, nullptr, &vk_swapchain));

		VkAssert(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_swapchain_image_count, nullptr));

		vk_swapchain_images = new VkImage[vk_swapchain_image_count];
		vk_swapchain_image_views = new VkImageView[vk_swapchain_image_count];

		VkAssert(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_swapchain_image_count, vk_swapchain_images));

		for (uint32_t i = 0; i < vk_swapchain_image_count; ++i)
		{
#pragma warning (push)
#pragma warning (disable:6385) // Reading invalid data from vk_swapchain_images
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
	#pragma warning (pop)
			};

			VkAssert(vkCreateImageView(vk_device, &viewInfo, nullptr, &vk_swapchain_image_views[i]));
		}
	}

	void CreateSwapchain()
	{
		VkSurfaceCapabilitiesKHR capabilities;
		VkAssert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physicaldevice, vk_surface, &capabilities));

		vk_swapchain_width = capabilities.currentExtent.width;
		vk_swapchain_height = capabilities.currentExtent.height;

		vk_swapchain_image_count = capabilities.minImageCount + 1;
		if (vk_swapchain_image_count > capabilities.maxImageCount) vk_swapchain_image_count = capabilities.maxImageCount;

		CreateSwapchainObjects();
		CalculateRenderScale();
	}

	void DestroySwapchain()
	{
		for (uint32_t i = 0; i < vk_swapchain_image_count; ++i)
		{
			vkDestroyImageView(vk_device, vk_swapchain_image_views[i], nullptr);
		}

		vkDestroySwapchainKHR(vk_device, vk_swapchain, nullptr);

		delete[] vk_swapchain_images;
		delete[] vk_swapchain_image_views;
	}

	void GetSwapchainFormat()
	{
		uint32_t surfaceFormatCount;
		VkAssert(vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physicaldevice, vk_surface, &surfaceFormatCount, nullptr));

		VkSurfaceFormatKHR *surfaceFormats = new VkSurfaceFormatKHR[surfaceFormatCount];
		VkAssert(vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physicaldevice, vk_surface, &surfaceFormatCount, surfaceFormats));

		if (surfaceFormatCount > 0)
		{
			vk_swapchain_format = surfaceFormats[0].format;
			vk_swapchain_colourspace = surfaceFormats[0].colorSpace;
		}

		for (uint32_t i = 0; i < surfaceFormatCount; ++i)
		{
#pragma warning (push)
#pragma warning (disable:6385)
			VkSurfaceFormatKHR &format = surfaceFormats[i];
#pragma warning (pop)

			if (format.format == VK_FORMAT_R8G8B8A8_SRGB && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
			{
				vk_swapchain_format = VK_FORMAT_R8G8B8A8_SRGB;
				vk_swapchain_colourspace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
				break;
			}
		}

		delete[] surfaceFormats;
	}

	void RecreateSwapchain()
	{
		DestroySwapchain();
		CreateSwapchain();
	}
}