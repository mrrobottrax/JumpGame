#include "pch.h"
#include "vk_debug.h"
#include "vulkan.h"
#include "vk_instance.h"
#include <console/console.h>

static VkDebugUtilsMessengerEXT vk_reportCallback;

void CreateDebugCallbacks()
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
		.pfnUserCallback = &DebugCallback,
	};

	VkAssert(vkCreateDebugUtilsMessengerEXT(vk_instance, &createInfo, nullptr, &vk_reportCallback));
}

void DestroyDebugCallbacks()
{
	vkDestroyDebugUtilsMessengerEXT(vk_instance, vk_reportCallback, nullptr);
}

VkBool32 DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
	void *pUserData)
{
	Log(pCallbackData->pMessage);
	OutputDebugStringA(pCallbackData->pMessage);
	OutputDebugStringA("\n");

	return VK_TRUE;
}
