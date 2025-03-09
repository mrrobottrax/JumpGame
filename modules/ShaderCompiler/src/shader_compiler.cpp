#include <pch.h>
#include <exceptions/exceptions.h>

using namespace std;

static bool CompileShaderFiles(const wchar_t *string)
{
	bool error = false;

	WIN32_FIND_DATA findData;
	HANDLE hFindFile = FindFirstFile(string, &findData);

	bool nextFile = hFindFile;
	while (nextFile)
	{
		wstring fullPath = wstring(L"data\\") + findData.cFileName;
		wstring outPath = fullPath + L".spv";

		printf("%ls\n", fullPath.c_str());

		bool shouldCompile = true;
		HANDLE hCompiledFile = CreateFile2(outPath.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, NULL);
		if (hCompiledFile)
		{
			FILETIME writeTime;
			if (!GetFileTime(hCompiledFile, NULL, NULL, &writeTime))
			{
				throw WindowsException("Failed to get file time");
			}

			// Don't compile when it hasn't changed
			if (CompareFileTime(&writeTime, &findData.ftLastWriteTime) == 1)
			{
				shouldCompile = false;
			}

			if (!CloseHandle(hCompiledFile))
			{
				throw WindowsException("Failed to close file");
			}
		}

		if (shouldCompile)
		{
			wstring cmd = L"glslc " + fullPath + L" -o " + outPath;
			printf("%ws\n", cmd.c_str());

			int out = _wsystem(cmd.c_str());
			if (out != 0)
			{
				error = true;
			}
		}

		nextFile = FindNextFile(hFindFile, &findData);
	}

	if (hFindFile && !FindClose(hFindFile))
	{
		throw WindowsException("Failed to close files");
	}

	return error;
}

bool CompileShaders()
{
	bool error = false;

	error |= CompileShaderFiles(L"data\\*.vert");
	error |= CompileShaderFiles(L"data\\*.frag");

	return !error;
}
