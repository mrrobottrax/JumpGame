#pragma once

// Memory leaks
#define _ATL_DEBUG_INTERFACES
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <combaseapi.h>
#include <mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <atlbase.h>
#include <audioclient.h>
#include <audiopolicy.h>
// Vulkan
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_win32.h>

// Modules
#include <pngloader.h>
#ifdef DEBUG
#include <shader_compiler.h>
#endif // DEBUG

// STD
#include <stdexcept>
#include <format>
#include <chrono>
#include <cassert>

using namespace std; // I know people hate this
