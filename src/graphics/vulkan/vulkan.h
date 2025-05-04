#pragma once

namespace Graphics::Vulkan
{
	void init();
	void shutdown();
	void wait_for_frame();
	void render_frame();

	void vk_assert(VkResult result);
}