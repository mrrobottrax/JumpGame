#include "stdafx.h"
#include "InStream.h"

bool InStream::ReadNextBit()
{
	bool bit = *pByte & (1 << bitOffset);

	++bitOffset;

	if (bitOffset >= 8)
	{
		++pByte;
		bitOffset = 0;
	}

	return bit;
}

unsigned int InStream::ReadUINT(unsigned char amt)
{
	if (amt > 8)
	{
		throw std::runtime_error("Can't read more than 8 bits into byte");
	}

	unsigned int integer = 0;
	bool bit;

	for (unsigned char i = 0; i < amt; ++i)
	{
		bit = ReadNextBit();
		integer |= bit << i;
	}

	return integer;
}

unsigned short InStream::ReadBits(unsigned short amt)
{
	/*if (amt > 8)
	{
		throw std::runtime_error("Can't read more than 8 bits into byte");
	}*/

	unsigned short byte = 0;
	bool bit;

	for (unsigned short i = 0; i < amt; ++i)
	{
		bit = ReadNextBit();
		byte |= bit << i;
	}

	return byte;
}
