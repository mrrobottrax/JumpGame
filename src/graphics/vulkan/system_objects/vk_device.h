#pragma once

namespace Graphics::Vulkan
{
    inline VkDevice vk_device;

    inline struct EnabledDeviceExtensions
    {
        bool memory_priority;
    } vk_device_extensions;

    void CreateDevice();
    void DestroyDevice();
}