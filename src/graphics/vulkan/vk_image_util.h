#pragma once

namespace Graphics::Vulkan
{
	void TransitionImage(VkImageLayout oldLayout, VkImageLayout newLayout, VkImage image);
}