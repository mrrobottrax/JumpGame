#pragma once

namespace Graphics::Vulkan
{
	void Init();
	void Shutdown();
	void WaitForFrame();
	void RenderFrame();

	void VkAssert(VkResult result);
}