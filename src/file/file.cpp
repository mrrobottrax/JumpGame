#include "pch.h"
#include "file.h"
#include "exceptions/exceptions.h"

struct MyOverlapped : OVERLAPPED
{
	DWORD errorCode;
	MyOverlapped()
	{
		memset(static_cast<OVERLAPPED *>(this), 0, sizeof(OVERLAPPED));
		errorCode = ERROR_SUCCESS;
	}
};

static void OnFileReadComplete(
	_In_    DWORD dwErrorCode,
	_In_    DWORD dwNumberOfBytesTransfered,
	_Inout_ LPOVERLAPPED lpOverlapped
)
{
	MyOverlapped *myOverlapped = static_cast<MyOverlapped *>(lpOverlapped);
	myOverlapped->errorCode = dwErrorCode;
	SetEvent(myOverlapped->hEvent);
}

FileHandle load_entire_file(const wchar_t name[])
{
	wstring fullpath = wstring(L"data\\") + name;

	CREATEFILE2_EXTENDED_PARAMETERS params = { 0 };
	params.dwSize = sizeof(params);
	params.dwFileFlags = FILE_FLAG_OVERLAPPED;

	HANDLE hFile = CreateFile2(fullpath.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &params);
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
		throw WindowsException("Couldn't create event");
	}

	MyOverlapped overlapped{};
	overlapped.hEvent = hEvent;

	if (!ReadFileEx(hFile, pData, (DWORD)size.QuadPart, &overlapped, OnFileReadComplete))
	{
		CloseHandle(hEvent);
		CloseHandle(hFile);
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
