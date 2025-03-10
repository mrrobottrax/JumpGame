#pragma once

void InitVulkan();
void EndVulkan();
void WaitForFrameVulkan();
void RenderFrameVulkan();

void VkAssert(VkResult result);