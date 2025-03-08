#pragma once

extern VkDevice vk_device;

class ShaderModuleWrapper
{
	VkShaderModule module;

public:
	ShaderModuleWrapper(VkShaderModule module) : module(module)
	{}

	~ShaderModuleWrapper()
	{
		vkDestroyShaderModule(vk_device, module, nullptr);
	}

	operator VkShaderModule &() { return module; }
};

ShaderModuleWrapper CreateShaderModule(const wchar_t name[]);