#pragma once

namespace Graphics
{
	namespace Vulkan
	{
		void Init();
		void Shutdown();
		void WaitForFrame();
		void RenderFrame();
	}

	inline void Init()
	{
		Vulkan::Init();
	}

	inline void Shutdown()
	{
		Vulkan::Shutdown();
	}

	inline void WaitForNextFrame()
	{
		Vulkan::WaitForFrame();
	}

	inline void Render()
	{
		Vulkan::RenderFrame();
	}
}