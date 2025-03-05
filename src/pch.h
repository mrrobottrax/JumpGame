#pragma once

// Memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Vulkan
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_win32.h>

// STD
#include <stdexcept>
#include <format>

using namespace std; // I know people hate this