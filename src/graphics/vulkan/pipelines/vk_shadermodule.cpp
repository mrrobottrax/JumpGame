#include "pch.h"
#include "vk_shadermodule.h"
#include <file/file.h>
#include <graphics/vulkan/vulkan.h>

namespace Graphics::Vulkan
{
	ShaderModuleWrapper create_shader_module(const wchar_t name[])
	{
		FileHandle file = load_entire_file(name);

		VkShaderModule module;
		VkShaderModuleCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.codeSize = file.Size(),
			.pCode = (uint32_t *)file.Data(),
		};

		vk_assert(vkCreateShaderModule(vk_device, &createInfo, nullptr, &module));

		return ShaderModuleWrapper(module);
	}
}