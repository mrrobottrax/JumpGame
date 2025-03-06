#pragma once

struct FileData
{
	void *pData;
	size_t size;
	unsigned int refCount;
};

class FileHandle
{
	FileData *pFile;

public:
	inline FileHandle(FileData *pFile) : pFile(pFile)
	{
		++pFile->refCount;
	}

	inline ~FileHandle()
	{
		--pFile->refCount;
	}
};

FileHandle LoadEntireFile(const char name[]);