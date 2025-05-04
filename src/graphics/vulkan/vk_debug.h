#pragma once

namespace Graphics::Vulkan
{
	void CreateDebugCallbacks();
	void DestroyDebugCallbacks();

	VkBool32 DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
		void *pUserData);
}