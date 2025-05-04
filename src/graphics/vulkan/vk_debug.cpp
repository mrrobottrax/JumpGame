#include "pch.h"
#include "vk_debug.h"
#include "vulkan.h"
#include <console/console.h>
#include "system_objects/vk_instance.h"

using namespace Console;

namespace Graphics::Vulkan
{
	static VkDebugUtilsMessengerEXT vk_reportCallback;

	void create_debug_callbacks()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo{
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
			.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
			.pfnUserCallback = &debug_callback,
		};

		vk_assert(vkCreateDebugUtilsMessengerEXT(vk_instance, &createInfo, nullptr, &vk_reportCallback));
	}

	void destroy_debug_callbacks()
	{
		vkDestroyDebugUtilsMessengerEXT(vk_instance, vk_reportCallback, nullptr);
	}

	VkBool32 debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
		void *pUserData)
	{
		log(pCallbackData->pMessage);
		OutputDebugStringA(pCallbackData->pMessage);
		OutputDebugStringA("\n");

		if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
		{
			return VK_FALSE;
		}

		return VK_TRUE;
	}
}