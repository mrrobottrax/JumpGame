#include "stdafx.h"
#include "PngLoader.h"
#include "Deflater.h"

constexpr unsigned int HEAD_SIZE = 8;

using namespace std;

struct ChunkData
{
	unsigned int length;
	char name[5];

	char const *pChunkInFile;
	char const *pData;
};

class ChunkList
{
public:
	unsigned int count;
	ChunkData *pChunks;

	ChunkList(unsigned int count) : count(count)
	{
		pChunks = new ChunkData[count];
	}

	~ChunkList()
	{
		delete[] pChunks;
	}
};

static uint32_t ReadUINT32(char const *const pNum)
{
	union
	{
		char bytes[4];
		uint32_t num;
	} number;

	number.bytes[0] = pNum[3];
	number.bytes[1] = pNum[2];
	number.bytes[2] = pNum[1];
	number.bytes[3] = pNum[0];

	return number.num;
}

static ChunkData LoadChunk(char const *const pChunk)
{
	ChunkData out = {};

	out.pChunkInFile = pChunk;
	out.pData = pChunk + 8;

	out.length = ReadUINT32(pChunk);

	for (int i = 0; i < 4; ++i)
	{
		out.name[i] = pChunk[4 + i];
	}
	out.name[4] = 0;

	return out;
}

static ChunkList GetChunks(void const *const pFile)
{
	char const *const &file = (char const *const)pFile;

	char const *pChunkInFile;

	constexpr unsigned int MAX_CHUNKS = 1024;
	unsigned int chunkCount;

	// Get number of chunks
	{
		pChunkInFile = file + 8;
		for (chunkCount = 0; chunkCount < MAX_CHUNKS; ++chunkCount)
		{
			ChunkData chunk = LoadChunk(pChunkInFile);
			pChunkInFile += 12 + chunk.length;

			if (strcmp(chunk.name, "IEND") == 0)
			{
				++chunkCount;
				break;
			}
		}

		if (chunkCount == MAX_CHUNKS)
		{
			throw std::runtime_error("Failed to read chunks");
		}
	}

	// Allocate array
	ChunkList list(chunkCount);

	// Fill array
	{
		pChunkInFile = file + 8;
		for (unsigned int i = 0; i < chunkCount; ++i)
		{
			ChunkData &chunk = list.pChunks[i];

			chunk = LoadChunk(pChunkInFile);
			pChunkInFile += 12 + chunk.length;
		}
	}

	return list;
}

static ChunkData *LocateChunk(ChunkData *pList, const char name[4])
{
	while (pList != 0)
	{
		if (strcmp(pList->name, "IEND") == 0)
		{
			return 0;
		}

		if (strcmp(pList->name, name) == 0)
		{
			return pList;
		}

		++pList;
	}

	return 0;
}

struct IHDRChunk
{
	unsigned int width, height;
	unsigned char bitDepth;
	unsigned char colorType;
	unsigned char compressionMethod;
	unsigned char filterMethod;
	unsigned char interlaceMethod;
};

static IHDRChunk ReadIHDR(ChunkData &chunk)
{
	IHDRChunk out = {};
	char const *pLoc = chunk.pData;

	out.width = ReadUINT32(pLoc); pLoc += 4;
	out.height = ReadUINT32(pLoc); pLoc += 4;

	out.bitDepth = *(pLoc++);
	out.colorType = *(pLoc++);
	out.compressionMethod = *(pLoc++);
	out.filterMethod = *(pLoc++);
	out.interlaceMethod = *(pLoc++);

	return out;
}

static void DecompressImageData(UncompressedImage &image, void const *pData)
{
	// Decompress DEFLATE stage
	OutStream outStream;
	Enflate(pData, outStream);

	size_t index = 0;

	// Deprocess each scanline
	outStream.ResetIndex();
	for (unsigned int row = 0; row < image.height; ++row)
	{
		unsigned char mode = outStream.ReadNextByte();

		switch (mode)
		{
			// SUB
			case 1:
			{
				unsigned char prevR = 0;
				unsigned char prevG = 0;
				unsigned char prevB = 0;
				unsigned char prevA = 0;

				for (unsigned int column = 0; column < image.width; ++column)
				{
					unsigned char r = outStream.ReadNextByte();
					unsigned char g = outStream.ReadNextByte();
					unsigned char b = outStream.ReadNextByte();
					unsigned char a = outStream.ReadNextByte();

					r += prevR;
					g += prevG;
					b += prevB;
					a += prevA;

					image.pData[index++] = r;
					image.pData[index++] = g;
					image.pData[index++] = b;
					image.pData[index++] = a;

					prevR = r;
					prevG = g;
					prevB = b;
					prevA = a;
				}
			}
			break;

			// UP
			case 2:
			{
				for (unsigned int column = 0; column < image.width; ++column)
				{
					unsigned char r = outStream.ReadNextByte();
					unsigned char g = outStream.ReadNextByte();
					unsigned char b = outStream.ReadNextByte();
					unsigned char a = outStream.ReadNextByte();

					unsigned char R = image.pData[((row - 1) * image.width + column) * 4 + 0];
					unsigned char G = image.pData[((row - 1) * image.width + column) * 4 + 1];
					unsigned char B = image.pData[((row - 1) * image.width + column) * 4 + 2];
					unsigned char A = image.pData[((row - 1) * image.width + column) * 4 + 3];

					r += R;
					g += G;
					b += B;
					a += A;

					image.pData[index++] = r;
					image.pData[index++] = g;
					image.pData[index++] = b;
					image.pData[index++] = a;
				}
			}
			break;

			// AVERAGE

			// PAETH
			case 4:
			{
				for (unsigned int column = 0; column < image.width; ++column)
				{
					unsigned char p[4] = {};
					for (int i = 0; i < 4; ++i)
					{
						p[i] = outStream.ReadNextByte();
					}

					unsigned char a[4] = {};
					unsigned char b[4] = {};
					unsigned char c[4] = {};

					// Left
					for (int i = 0; i < 4; ++i)
					{
						if (column == 0)
						{
							a[i] = 0;
						}
						else
						{
							a[i] = image.pData[(row * image.width + column - 1) * 4 + i];
						}
					}

					// Top
					for (int i = 0; i < 4; ++i)
					{
						b[i] = image.pData[((row - 1) * image.width + column) * 4 + i];
					}

					// Top Left
					for (int i = 0; i < 4; ++i)
					{
						if (column == 0)
						{
							c[i] = 0;
						}
						else
						{
							c[i] = image.pData[((row - 1) * image.width + column - 1) * 4 + i];
						}
					}

					for (int i = 0; i < 4; ++i)
					{
						unsigned char x = 0;

						unsigned int j = a[i] + b[i] - c[i];
						unsigned int ja = abs((int)(j - a[i]));
						unsigned int jb = abs((int)(j - b[i]));
						unsigned int jc = abs((int)(j - c[i]));

						if (ja <= jb && ja <= jc) x = a[i];
						else if (jb <= jc)        x = b[i];
						else                      x = c[i];

						p[i] += x;

						image.pData[index++] = p[i];
					}
				}
			}
			break;

			default:
				throw std::runtime_error("Process mode not supported");
				break;
		}
	}
}

static UncompressedImage ProcessChunks(ChunkData *pChunks, unsigned int chunkCount)
{
	// Find IHDR and get initial data
	ChunkData &ihdrData = *LocateChunk(pChunks, "IHDR");

	IHDRChunk ihdr = ReadIHDR(ihdrData);

	UncompressedImage image(ihdr.width, ihdr.height);

	if (ihdr.bitDepth != 8 || ihdr.colorType != 6 || ihdr.compressionMethod != 0
		|| ihdr.filterMethod != 0 || ihdr.interlaceMethod != 0)
	{
		throw std::runtime_error("Image format not supported");
	}

	// Decompress image data
	ChunkData *pIdatData = LocateChunk(pChunks, "IDAT");

	if (LocateChunk(pIdatData + 1, "IDAT"))
	{
		throw std::runtime_error("Multiple IDAT chunks not supported");
	}

	DecompressImageData(image, pIdatData->pData);

	return image;
}

UncompressedImage LoadAndUncompressPNG(wchar_t const *const path)
{
	ifstream file;
	file.open(path, ios::in | ios::ate | ios::binary);
	streampos end = file.tellg();

	if (end == 0 || end == -1)
	{
		throw std::runtime_error("Failed to open file");
	}

	void *pFileData = malloc(end);

	file.seekg(0);
	file.read((char *)pFileData, end);
	file.close();

	try
	{
		UncompressedImage image = UncompressPNG(pFileData);
		free(pFileData);
		return image;
	}
	catch (std::exception &e)
	{
		free(pFileData);
		throw e;
	}
}

UncompressedImage UncompressPNG(void const *const data)
{
	ChunkList list = GetChunks(data);

	UncompressedImage image = ProcessChunks(list.pChunks, list.count);
	return image;
}
