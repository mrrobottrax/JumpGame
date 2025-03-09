#include "stdafx.h"
#include "Deflater.h"
#include "InStream.h"
#include "OutStream.h"
#include "Huffman.h"

enum BlockMethod : char
{
	STORED = 0,
	STATIC_HUFFMAN = 1,
	DYNAMIC_HUFFMAN = 2,
	RESERVED = 3,
};

struct BlockData
{
	BlockMethod method;
	bool last;
};

struct StreamData
{
	unsigned char method;
	unsigned char flags;
	unsigned int windowSize;

	char const *pData;
};

static BlockData ReadBlockHeader(InStream &stream)
{
	BlockData block = {};

	block.last = stream.ReadNextBit();
	block.method = (BlockMethod)stream.ReadBits(2);

	return block;
}

static StreamData ReadStreamData(char const *pStream)
{
	StreamData stream = {};

	stream.method = (*pStream) & 0b00001111;

	unsigned char cinfo = (*pStream) >> 4;
	stream.windowSize = 1 << (cinfo + 8);

	stream.flags = *(pStream + 1);

	stream.pData = pStream + 2;

	// FDICT
	if (stream.flags & (1 << 5))
	{
		stream.pData += 4;
		throw std::runtime_error("FDICT flag not supported");
	}

	return stream;
}

static void DecompressSTOREDBlock(InStream &stream, OutStream &out)
{
	unsigned int length = stream.ReadUINT(16);

	for (unsigned int i = 0; i < length; ++i)
	{
		out.AppendByte(stream.ReadBits(8));
	}
}

void Enflate(void const *const pData1, OutStream &outStream)
{
	char const *const &pData = (char *)pData1;

	// Get stream header (zlib)
	StreamData streamData = ReadStreamData(pData);
	{
		if (streamData.method != 8)
		{
			throw std::runtime_error("Unsupported compression method");
		}

		if (streamData.flags & 1 << 5)
		{
			throw std::runtime_error("FDICT flag not supported");
		}
	}

	InStream stream(streamData.pData);

	// Read each block (DEFLATE)
	while (true)
	{
		BlockData blockData = ReadBlockHeader(stream);
		switch (blockData.method)
		{
			case STORED:
				DecompressSTOREDBlock(stream, outStream);
				break;
			case STATIC_HUFFMAN:
				DecompressStreamStaticTrees(stream, outStream);
				break;
			case DYNAMIC_HUFFMAN:
				Tree<LITERAL_LENGTH_ALPHABET_SIZE> literalLength;
				Tree<DISTANCE_ALPHABET_SIZE> distance;
				ReadTrees(stream, literalLength, distance);
				DecompressStream(stream, outStream, literalLength, distance);
				break;
			default:
				throw std::runtime_error("Unsupported type");
		}

		if (blockData.last)
		{
			break;
		}
	}
}