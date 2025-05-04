#pragma once

namespace Graphics::Vulkan
{
	void create_debug_callbacks();
	void destroy_debug_callbacks();

	VkBool32 debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
		void *pUserData);
}