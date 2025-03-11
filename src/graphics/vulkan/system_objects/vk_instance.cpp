#include "pch.h"
#include "vk_instance.h"
#include "console/console.h"
#include <graphics/vulkan/vk_debug.h>
#include <graphics/vulkan/vulkan.h>
#include "graphics/vulkan/extensions/vk_ext_debugutils.h"

#ifdef DEBUG
constexpr const char *REQUIRED_LAYERS[] = {
	"VK_LAYER_KHRONOS_validation",
	"VK_LAYER_KHRONOS_synchronization2",
};

constexpr const char *REQUIRED_EXTENSIONS[] = {
	"VK_EXT_debug_utils",
	"VK_KHR_surface",
	"VK_KHR_win32_surface",
};
#else
constexpr const char *REQUIRED_LAYERS[] = {
	"VK_LAYER_KHRONOS_synchronization2",
};

constexpr const char *REQUIRED_EXTENSIONS[] = {
	"VK_KHR_surface",
	"VK_KHR_win32_surface",
};
#endif // DEBUG

void CreateInstance()
{
	uint32_t layerCount;
	VkAssert(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

	VkLayerProperties *layers = new VkLayerProperties[layerCount];
	VkAssert(vkEnumerateInstanceLayerProperties(&layerCount, layers));

	Log("AVAILABLE LAYERS:");
#pragma warning (push)
#pragma warning (disable:6385)
	for (uint32_t i = 0; i < layerCount; ++i)
	{
		VkLayerProperties &layer = layers[i];

		Log(layer.layerName);

		uint32_t extensionCount;
		VkAssert(vkEnumerateInstanceExtensionProperties(layer.layerName, &extensionCount, nullptr));

		VkExtensionProperties *extensions = new VkExtensionProperties[extensionCount];
		VkAssert(vkEnumerateInstanceExtensionProperties(layer.layerName, &extensionCount, extensions));

		for (uint32_t j = 0; j < extensionCount; ++j)
		{
			VkExtensionProperties &extension = extensions[j];

			Log("    %s", extension.extensionName);
		}

		delete[] extensions;
	}

	Log("AVAILABLE EXTENSIONS:");

	uint32_t extensionCount;
	VkAssert(vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, nullptr));

	VkExtensionProperties *extensions = new VkExtensionProperties[extensionCount];
	VkAssert(vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions));

	for (uint32_t j = 0; j < extensionCount; ++j)
	{
		VkExtensionProperties &extension = extensions[j];

		Log("    %s", extension.extensionName);
	}

	delete[] extensions;
#pragma warning (pop)

	VkApplicationInfo appInfo{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Jump Game",
		.applicationVersion = 0,
		.pEngineName = "MAGE",
		.engineVersion = 0,
		.apiVersion = VK_MAKE_API_VERSION(0, 1, 3, 0),
	};

	VkInstanceCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = _countof(REQUIRED_LAYERS),
		.ppEnabledLayerNames = REQUIRED_LAYERS,
		.enabledExtensionCount = _countof(REQUIRED_EXTENSIONS),
		.ppEnabledExtensionNames = REQUIRED_EXTENSIONS,
	};

	VkAssert(vkCreateInstance(&createInfo, nullptr, &vk_instance));

	delete[] layers;

#ifdef DEBUG
	GetDebugUtilsFunctionPointers();
	CreateDebugCallbacks();
#endif // DEBUG
}

void DestroyInstance()
{
	DestroyDebugCallbacks();
	vkDestroyInstance(vk_instance, nullptr);
}