#pragma once

class UncompressedImage
{
public:
	unsigned const int width, height;
	unsigned char *pData = 0;

	UncompressedImage(UncompressedImage &&image) noexcept :
		width(image.width), height(image.height), pData(image.pData)
	{
		image.pData = nullptr;
	}

	UncompressedImage(unsigned int width, unsigned int height) :
		width(width), height(height), pData(new unsigned char[width * height * 4])
	{}

	~UncompressedImage()
	{
		if (pData) delete[] pData;
	}
};

UncompressedImage LoadAndUncompressPNG(wchar_t const *const path);
UncompressedImage UncompressPNG(void const *const data);