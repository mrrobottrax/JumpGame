#pragma once

class UncompressedImage
{
public:
	unsigned int width, height;
	unsigned char *pData = 0;

	UncompressedImage(UncompressedImage &&image) noexcept :
		width(image.width), height(image.height), pData(image.pData)
	{
		image.pData = nullptr;
	}

	UncompressedImage(const UncompressedImage &image) noexcept :
		width(image.width), height(image.height), pData(image.pData)
	{}

	UncompressedImage(unsigned int width, unsigned int height) :
		width(width), height(height), pData(new unsigned char[width * height * 4])
	{}

	UncompressedImage() :
		width(0), height(0), pData(nullptr)
	{}

	~UncompressedImage()
	{
		delete[] pData;
		pData = nullptr;
	}

	UncompressedImage &operator =(UncompressedImage &&img) noexcept
	{
		width = img.width;
		height = img.height;
		pData = img.pData;

		img.pData = nullptr;

		return *this;
	}
};

UncompressedImage LoadAndUncompressPNG(wchar_t const *const path);
UncompressedImage UncompressPNG(void const *const data);