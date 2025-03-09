#pragma once

class InStream
{
	char const *pByte;
	unsigned char bitOffset;

public:
	InStream(void const *pData) : pByte((char const *)pData), bitOffset(0)
	{}

	bool ReadNextBit();
	unsigned char ReadBits(unsigned char amt);
	unsigned int ReadUINT(unsigned char amt);
};