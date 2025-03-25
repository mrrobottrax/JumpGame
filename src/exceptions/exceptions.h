#pragma once

#include <exception>

class WindowsException : private std::exception
{
public:
	inline WindowsException(const char c[]) noexcept : std::exception(c)
	{}

	std::string msg() const;
};

class VulkanException : private std::exception
{
	VkResult result;

public:
	inline VulkanException(const char c[], VkResult result) noexcept : std::exception(c), result(result)
	{}

	std::string msg() const;
};

void ThrowIfNull(HANDLE handle);
void ThrowIfFailed(HRESULT hr);
void ThrowIfFailed(HRESULT hr, const char message[]);