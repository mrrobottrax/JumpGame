#pragma once

void InitVulkan();
void EndVulkan();
void RenderFrameVulkan();

void VkAssert(VkResult result);