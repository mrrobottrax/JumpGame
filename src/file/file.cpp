#include "pch.h"
#include "file.h"
#include "exceptions/exceptions.h"

static void OnFileReadComplete(_In_    DWORD dwErrorCode,
							   _In_    DWORD dwNumberOfBytesTransfered,
							   _Inout_ LPOVERLAPPED lpOverlapped
)
{
	if (!SetEvent(lpOverlapped->hEvent))
	{
		throw WindowsException("Failed to set event.");
	}
}

FileHandle LoadEntireFile(const wchar_t name[])
{
	wstring fullpath = wstring(L"data\\") + name;

	HANDLE hFile = CreateFile2(fullpath.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, NULL);
	if (hFile == NULL)
	{
		throw new WindowsException("Couldn't file handle.");
	}

	LARGE_INTEGER size;
	if (!GetFileSizeEx(hFile, &size))
	{
		throw WindowsException("Couldn't get file size.");
	}

	void *pData = malloc(size.QuadPart);

	FileData *data = new FileData{
		.pData = pData,
		.size = (size_t)size.QuadPart,
		.refCount = 0,
	};

	HANDLE hEvent = CreateEventEx(NULL, L"Open File Event", CREATE_EVENT_MANUAL_RESET, SYNCHRONIZE | EVENT_MODIFY_STATE);
	if (hEvent == NULL)
	{
		throw new WindowsException("Couldn't create event");
	}

	OVERLAPPED overlapped{
		.hEvent = hEvent,
	};

	if (!ReadFileEx(hFile, pData, (DWORD)size.QuadPart, &overlapped, OnFileReadComplete))
	{
		throw WindowsException("Couldn't read file.");
	}

	DWORD result;
	do
	{
		result = WaitForSingleObjectEx(hEvent, INFINITE, TRUE);
	} while (result == WAIT_IO_COMPLETION);

	CloseHandle(hEvent);
	CloseHandle(hFile);

	if (result != WAIT_OBJECT_0)
	{
		throw WindowsException("Couldn't wait for read.");
	}

	return FileHandle(data);
}
