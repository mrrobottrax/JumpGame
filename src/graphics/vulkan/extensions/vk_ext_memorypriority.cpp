#include "pch.h"
#include "vk_ext_memorypriority.h"
#include "graphics/vulkan/system_objects/vk_instance.h"

static PFN_vkSetDeviceMemoryPriorityEXT pfn_vkSetDeviceMemoryPriorityEXT;
VKAPI_ATTR void VKAPI_CALL vkSetDeviceMemoryPriorityEXT(
	VkDevice                                    device,
	VkDeviceMemory                              memory,
	float                                       priority)
{
	return pfn_vkSetDeviceMemoryPriorityEXT(device, memory, priority);
}

void Graphics::Vulkan::get_memory_priority_function_pointers()
{
	pfn_vkSetDeviceMemoryPriorityEXT = (PFN_vkSetDeviceMemoryPriorityEXT)vkGetInstanceProcAddr(vk_instance, "vkSetDeviceMemoryPriorityEXT");
}
