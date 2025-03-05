#pragma once

#include <exception>

class WindowsException : private std::exception
{
public:
	inline WindowsException(const char c[]) noexcept : std::exception(c)
	{}

	std::string msg() const;
};

void ThrowIfFailed(HRESULT hr);
void ThrowIfFailed(HRESULT hr, const char message[]);