#include "pch.h"
#include "vk_surface.h"
#include <application/application.h>
#include <window/window.h>
#include "vk_instance.h"

namespace Graphics::Vulkan
{
	void CreateSurface()
	{
		VkWin32SurfaceCreateInfoKHR createInfo{
			.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			.hinstance = hInstance,
			.hwnd = hwnd,
		};

		vkCreateWin32SurfaceKHR(vk_instance, &createInfo, nullptr, &vk_surface);
	}

	void DestroySurface()
	{
		vkDestroySurfaceKHR(vk_instance, vk_surface, nullptr);
	}
}