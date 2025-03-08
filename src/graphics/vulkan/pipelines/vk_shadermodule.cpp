#include "pch.h"
#include "vk_shadermodule.h"
#include <file/file.h>
#include <graphics/vulkan/vk_device.h>
#include <graphics/vulkan/vulkan.h>

ShaderModuleWrapper CreateShaderModule(const wchar_t name[])
{
	FileHandle file = LoadEntireFile(name);

	VkShaderModule module;
	VkShaderModuleCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = file.Size(),
		.pCode = (uint32_t *)file.Data(),
	};

	VkAssert(vkCreateShaderModule(vk_device, &createInfo, nullptr, &module));

	return ShaderModuleWrapper(module);
}