#pragma once

class OutStream
{
	constexpr static size_t OUT_STREAM_PAGE_SIZE = 1 << 15;

	struct Page
	{
		unsigned char data[OUT_STREAM_PAGE_SIZE];

		Page *pPrev;
		Page *pNext;
	};

	Page *pFirstPage;

	size_t index = 0;
	size_t length = 0;

public:
	OutStream()
	{
		pFirstPage = (Page *)malloc(sizeof(Page));

		if (pFirstPage == 0)
		{
			throw std::runtime_error("Failed to alloc page");
			return;
		}

		pFirstPage->pPrev = 0;
		pFirstPage->pNext = 0;
	}

	~OutStream()
	{
		Page *pPage = pFirstPage;
		while (pPage->pNext)
		{
			pPage = pPage->pNext;
		}

		while (pPage)
		{
			Page *cache = pPage;
			pPage = pPage->pPrev;

			free(cache);
		}
	}

	void AppendByte(unsigned char);
	void CopyPrev(unsigned int length, unsigned int distance);

	inline void SetEnd() { length = index; }
	inline void ResetIndex() { index = 0; }
	unsigned char ReadNextByte();
	unsigned char ReadByte(size_t index);

private:
	unsigned char ReadBack(unsigned int distance);
};