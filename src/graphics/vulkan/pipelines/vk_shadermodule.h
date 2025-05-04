#pragma once

namespace Graphics::Vulkan
{
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

	ShaderModuleWrapper create_shader_module(const wchar_t name[]);
}