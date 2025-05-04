#include "pch.h"
#include "vk_surface.h"
#include <application/application.h>
#include <window/window.h>
#include "vk_instance.h"

namespace Graphics::Vulkan
{
	void create_surface()
	{
		VkWin32SurfaceCreateInfoKHR createInfo{
			.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			.hinstance = Application::hInstance,
			.hwnd = hwnd,
		};

		vkCreateWin32SurfaceKHR(vk_instance, &createInfo, nullptr, &vk_surface);
	}

	void destroy_surface()
	{
		vkDestroySurfaceKHR(vk_instance, vk_surface, nullptr);
	}
}