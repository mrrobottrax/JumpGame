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
		if (pFile->refCount <= 0)
		{
			free(pFile->pData);
			delete pFile;
		}
	}

	inline size_t Size() { return pFile->size; }
	inline const void *Data() { return pFile->pData; }
};

FileHandle load_entire_file(const wchar_t name[]);