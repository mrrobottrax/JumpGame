#include "pch.h"
#include "vk_instance.h"
#include "console/console.h"
#include "vulkan.h"

VkInstance vk_instance;

#ifdef DEBUG
constexpr const char *REQUIRED_LAYERS[] = {
	"VK_LAYER_KHRONOS_validation",
	"VK_LAYER_KHRONOS_synchronization2",
};
#else
constexpr const char *REQUIRED_LAYERS[] = {
	"VK_LAYER_KHRONOS_synchronization2",
};
#endif // DEBUG

void CreateInstance()
{
	uint32_t layerCount;
	VkAssert(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

	VkLayerProperties *layers = new VkLayerProperties[layerCount];
	VkAssert(vkEnumerateInstanceLayerProperties(&layerCount, layers));

	Log("AVAILABLE LAYERS:");
	for (uint32_t i = 0; i < layerCount; ++i)
	{
#pragma warning (push)
#pragma warning (disable:6385)
		VkLayerProperties &layer = layers[i];
#pragma warning (pop)

		Log(layer.layerName);
	}

	VkInstanceCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.enabledLayerCount = _countof(REQUIRED_LAYERS),
		.ppEnabledLayerNames = REQUIRED_LAYERS,
	};

	VkAssert(vkCreateInstance(&createInfo, nullptr, &vk_instance));

	delete[] layers;
}

void DestroyInstance()
{
	vkDestroyInstance(vk_instance, nullptr);
}