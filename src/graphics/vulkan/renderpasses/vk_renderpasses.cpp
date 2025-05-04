#include "pch.h"
#include "vk_renderpasses.h"
#include "vk_objects_pass.h"

namespace Graphics::Vulkan
{
	void create_render_passes()
	{
		create_objects_pass();
	}

	void destroy_render_passes()
	{
		destroy_objects_pass();
	}
}
