#pragma once

#include <exception>
#include "window/window.h"

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

void throw_if_null(HANDLE handle);
void throw_if_failed(HRESULT hr);
void throw_if_failed(HRESULT hr, const char message[]);

inline void alert(std::exception &e)
{
	try
	{
		throw e;
	}
	catch (WindowsException &e)
	{
		printf(e.msg().c_str());
		MessageBoxA(Window::hwnd, e.msg().c_str(), NULL, MB_SYSTEMMODAL);
		system("pause");
	}
	catch (VulkanException &e)
	{
		printf(e.msg().c_str());
		MessageBoxA(Window::hwnd, e.msg().c_str(), NULL, MB_SYSTEMMODAL);
		system("pause");
	}
	catch (std::exception &e)
	{
		printf("%s\n", e.what());
		MessageBoxA(Window::hwnd, e.what(), NULL, MB_SYSTEMMODAL);
		system("pause");
	}
	catch (...)
	{
		printf("Unknown error");
		MessageBoxA(Window::hwnd, "Unknown error", NULL, MB_SYSTEMMODAL);
		system("pause");
	}
}