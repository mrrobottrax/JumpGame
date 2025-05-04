#pragma once

namespace Graphics
{
	namespace Vulkan
	{
		void init();
		void shutdown();
		void wait_for_frame();
		void render_frame();
	}

	inline void init()
	{
		Vulkan::init();
	}

	inline void shutdown()
	{
		Vulkan::shutdown();
	}

	inline void WaitForNextFrame()
	{
		Vulkan::wait_for_frame();
	}

	inline void Render()
	{
		Vulkan::render_frame();
	}
}