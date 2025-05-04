#include "pch.h"
#include "vk_renderpasses.h"
#include "vk_objects_pass.h"

namespace Graphics::Vulkan
{
	void CreateRenderPasses()
	{
		CreateObjectsPass();
	}

	void DestroyRenderPasses()
	{
		DestroyObjectsPass();
	}
}
