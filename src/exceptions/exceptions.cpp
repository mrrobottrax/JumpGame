#include "pch.h"
#include "exceptions.h"

using namespace std;

std::string WindowsException::msg() const
{
	LPCTSTR strErrorMessage = NULL;

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		GetLastError(),
		0,
		(LPWSTR)&strErrorMessage,
		0,
		NULL);

	size_t len;
	wcstombs_s(&len, nullptr, 0, strErrorMessage, 0);
	++len;

	char *pBuffer = new char[len];

	wcstombs_s(nullptr, pBuffer, len, strErrorMessage, len - 1);

	std::string str(pBuffer);
	delete[] pBuffer;

	return format("{}\n{}", exception::what(), str);
}

std::string VulkanException::msg() const
{
	std::string str = string_VkResult(result);

	return format("{}\n{}", exception::what(), str);
}

void throw_if_null(HANDLE handle)
{
	if (handle == NULL)
	{
		throw WindowsException("ERR:");
	}
}

void throw_if_failed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw WindowsException("ERR:");
	}
}

void throw_if_failed(HRESULT hr, const char message[])
{
	if (FAILED(hr))
	{
		throw WindowsException(message);
	}
}