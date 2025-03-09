#include "stdafx.h"
#include "OutStream.h"

void OutStream::AppendByte(unsigned char val)
{
	size_t pageNum = index / OUT_STREAM_PAGE_SIZE;

	Page *pPage = pFirstPage;
	for (int i = 0; i < pageNum; ++i)
	{
		pPage = pPage->pNext;
	}

	pPage->data[index % OUT_STREAM_PAGE_SIZE] = val;

	++index;
	if (index % OUT_STREAM_PAGE_SIZE == 0)
	{
		pPage->pNext = (Page *)malloc(sizeof(Page));

		if (pPage->pNext == 0)
		{
			throw std::runtime_error("Failed to alloc page");
		}

		Page &newPage = *pPage->pNext;
		newPage.pNext = 0;

		pPage->pNext = &newPage;
		newPage.pPrev = pPage;
	}
}

unsigned char OutStream::ReadBack(unsigned int distance)
{
	size_t index = this->index - distance;

	if (index < 0) return 0;

	size_t pageNum = index / OUT_STREAM_PAGE_SIZE;

	Page *pPage = pFirstPage;
	for (int i = 0; i < pageNum; ++i)
	{
		pPage = pPage->pNext;
	}

	return pPage->data[index % OUT_STREAM_PAGE_SIZE];
}

void OutStream::CopyPrev(unsigned int length, unsigned int distance)
{
	for (unsigned int i = 0; i < length; ++i)
	{
		AppendByte(ReadBack(distance));
	}
}

unsigned char OutStream::ReadByte(size_t index)
{
	size_t pageNum = index / OUT_STREAM_PAGE_SIZE;

	Page *pPage = pFirstPage;
	for (int i = 0; i < pageNum; ++i)
	{
		pPage = pPage->pNext;
	}

	return pPage->data[index % OUT_STREAM_PAGE_SIZE];
}

unsigned char OutStream::ReadNextByte()
{
	return ReadByte(index++);
}
