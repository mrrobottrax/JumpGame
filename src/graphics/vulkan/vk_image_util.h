#pragma once

namespace Graphics::Vulkan
{
	void transition_image(VkImageLayout oldLayout, VkImageLayout newLayout, VkImage image);
}