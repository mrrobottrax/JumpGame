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

// STD
#include <stdexcept>
#include <format>