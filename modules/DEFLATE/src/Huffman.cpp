#include "stdafx.h"
#include "Huffman.h"
#include "InStream.h"
#include "OutStream.h"

template<unsigned int alphabetSize, unsigned int maxCodeLength>
static constexpr void GenerateCodesFromLengths(Tree<alphabetSize, maxCodeLength> &tree)
{
	// Init
	unsigned short bl_count[maxCodeLength + 1] = {};

	// Get frequency of each length
	for (int i = 0; i < alphabetSize; ++i)
	{
		unsigned char length = tree.codes[i].length;
		++bl_count[length];
	}
	bl_count[0] = 0;

	// Get smallest code for each length
	code_t code = 0;
	code_t next_code[maxCodeLength + 1] = {};
	for (unsigned char bits = 1; bits <= maxCodeLength; ++bits)
	{
		code = (code + bl_count[bits - 1]) << 1;
		next_code[bits] = code;
	}

	// Fill out all codes of all lengths
	for (int i = 0; i < alphabetSize; ++i)
	{
		unsigned char &length = tree.codes[i].length;
		if (length != 0)
		{
			tree.codes[i].code = next_code[length];
			++next_code[length];
		}
	}
}

static consteval Tree<LITERAL_LENGTH_ALPHABET_SIZE> GetStaticTreeBitLengths()
{
	Tree<LITERAL_LENGTH_ALPHABET_SIZE> tree = {};

	unsigned int i = 0;
	for (; i <= 143; ++i)
	{
		tree.codes[i].length = 8;
	}
	for (; i <= 255; ++i)
	{
		tree.codes[i].length = 9;
	}
	for (; i <= 279; ++i)
	{
		tree.codes[i].length = 7;
	}
	for (; i <= 287; ++i)
	{
		tree.codes[i].length = 8;
	}

	for (i = 0; i <= 287; ++i)
	{
		tree.codes[i].value = i;
	}


	return tree;
}

template<unsigned int alphabetSize, unsigned int maxCodeLength>
static constexpr void GenerateTree(Tree<alphabetSize, maxCodeLength> &tree)
{
	GenerateCodesFromLengths(tree);

	unsigned int(&buckets)[maxCodeLength + 1] = tree.buckets;
	for (int i = 0; i < _countof(buckets); ++i)
	{
		buckets[i] = -1;
	}

	Code pCodesOG[alphabetSize] = {};
	for (int i = 0; i < alphabetSize; ++i)
	{
		pCodesOG[i] = tree.codes[i];
	}

	int out = 0;
	for (int bits = 0; bits <= maxCodeLength; ++bits)
	{
		for (int symbol = 0; symbol < alphabetSize; ++symbol)
		{
			if (pCodesOG[symbol].length == bits)
			{
				if (buckets[bits] == -1)
				{
					buckets[bits] = out;
				}

				tree.codes[out] = pCodesOG[symbol];

				++out;
			}
		}
	}
}

static consteval Tree<LITERAL_LENGTH_ALPHABET_SIZE> GenerateStaticLiteralLengthTree()
{
	Tree<LITERAL_LENGTH_ALPHABET_SIZE> tree = GetStaticTreeBitLengths();

	GenerateTree(tree);

	return tree;
}

static consteval Tree<DISTANCE_ALPHABET_SIZE> GenerateStaticDistanceTree()
{
	Tree<DISTANCE_ALPHABET_SIZE> tree = {};

	for (int i = 0; i < _countof(tree.buckets); ++i)
	{
		tree.buckets[i] = -1;
	}

	tree.buckets[5] = 0;
	for (int i = 0; i < DISTANCE_ALPHABET_SIZE; ++i)
	{
		tree.codes[i].value = i;
		tree.codes[i].length = 5;
		tree.codes[i].code = i;
	}

	return tree;
}

constexpr Tree staticLiteralLengthTree = GenerateStaticLiteralLengthTree();
constexpr Tree staticDistanceTree = GenerateStaticDistanceTree();

void DecompressStreamStaticTrees(InStream &stream, OutStream &outStream)
{
	DecompressStream(stream, outStream, staticLiteralLengthTree, staticDistanceTree);
}

template<unsigned int alphabetLength, unsigned char maxCodeLength>
static Code ReadCode(InStream &stream, Tree<alphabetLength, maxCodeLength> const &tree)
{
	code_t readCode = 0;

	int length;
	for (length = 1; length <= maxCodeLength; ++length)
	{
		// Read next bit in code
		bool bit = stream.ReadNextBit();
		readCode = readCode << 1;
		readCode |= (code_t)bit;

		// Check if this is a leaf
		unsigned int start = tree.buckets[length];

		// No leafs of this length
		if (start == -1)
		{
			continue;
		}

		code_t first = tree.codes[start].code;
		unsigned int bitOffset = readCode - first;

		Code const &arrayCode = tree.codes[start + bitOffset];

		// Leaf
		if (arrayCode.length == length && arrayCode.code == readCode)
		{
			return arrayCode;
		}
	}

	throw std::runtime_error("Failed to find code");
}

static void ReadLengths(InStream &stream,
						unsigned char *pLengths,
						Tree<CODELENGTHS_ALPHABET_SIZE, MAX_CODELENGTH_LENGTH> codelengthsTree,
						unsigned int lengthsCount)
{
	for (unsigned int i = 0; i < lengthsCount; ++i)
	{
		pLengths[i] = 0;
	}

	for (unsigned int i = 0; i < lengthsCount;)
	{
		Code code = ReadCode(stream, codelengthsTree);

		unsigned char repeatLength = 0;
		unsigned char repeatValue = 0;

		if (code.value <= 15)
		{
			pLengths[i++] = (unsigned char)code.value;
		}
		else if (code.value == 16)
		{
			repeatLength = stream.ReadBits(2) + 3;

			if (i != 0)
			{
				repeatValue = pLengths[i - 1];
			}
		}
		else if (code.value == 17)
		{
			repeatLength = stream.ReadBits(3) + 3;
		}
		else if (code.value == 18)
		{
			repeatLength = stream.ReadBits(7) + 11;
		}
		else
		{
			throw std::runtime_error("WTF!");
		}

		if (i + repeatLength > lengthsCount)
		{
			throw std::runtime_error("Out of container");
		}

		for (int j = 0; j < repeatLength; ++j)
		{
			pLengths[i++] = repeatValue;
		}
	}
}

void ReadTrees(InStream &stream, Tree<LITERAL_LENGTH_ALPHABET_SIZE> &literalLengthTree, Tree<DISTANCE_ALPHABET_SIZE> &distanceTree)
{
	unsigned short HLIT = stream.ReadBits(5);
	unsigned short HDIST = stream.ReadBits(5);
	unsigned short HCLEN = stream.ReadBits(4);

	// Init codelengths alphabet
	Tree<CODELENGTHS_ALPHABET_SIZE, MAX_CODELENGTH_LENGTH> codelengthsTree{};

	constexpr unsigned char lengthsOrder[CODELENGTHS_ALPHABET_SIZE] = {
		16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

#define USE_LENGTH_ORDER_ALT

	for (int i = 0; i < _countof(codelengthsTree.codes); ++i)
	{
		codelengthsTree.codes[i].length = 0;

	#ifdef USE_LENGTH_ORDER_ALT
		codelengthsTree.codes[i].value = i;
	#else
		codelengthsTree.codes[i].value = lengthsOrder[i];
	#endif // USE_LENGTH_ORDER_ALT

	}

	// Read lengths for codelength alphabet
	for (int i = 0; i < HCLEN + 4; ++i)
	{
		unsigned int length = stream.ReadBits(3);

	#ifdef USE_LENGTH_ORDER_ALT
		codelengthsTree.codes[lengthsOrder[i]].length = length;
	#else
		codelengthsTree.codes[i].length = length;
	#endif // USE_LENGTH_ORDER_ALT

	}

	GenerateTree(codelengthsTree);

	class Lengths
	{
	public:
		unsigned int count;
		unsigned char *pLengths;

		Lengths(unsigned int count) : count(count)
		{
			pLengths = new unsigned char[count];
		}

		~Lengths()
		{
			delete[] pLengths;
		}
	};

	Lengths lengths(HLIT + HDIST + 258);
	ReadLengths(stream, lengths.pLengths, codelengthsTree, lengths.count);

	int index = 0;

	// Create literal/lengths tree
	for (int i = 0; i < LITERAL_LENGTH_ALPHABET_SIZE; ++i)
	{
		literalLengthTree.codes[i].value = i;
		literalLengthTree.codes[i].length = 0;
		literalLengthTree.codes[i].code = 0;
	}
	for (; index < HLIT + 257; ++index)
	{
		literalLengthTree.codes[index].length = lengths.pLengths[index];
	}
	GenerateTree(literalLengthTree);

	// Create distances tree
	for (int i = 0; i < DISTANCE_ALPHABET_SIZE; ++i)
	{
		distanceTree.codes[i].value = i;
		distanceTree.codes[i].length = 0;
		distanceTree.codes[i].code = 0;
	}
	for (; index < HLIT + HDIST + 258; ++index)
	{
		distanceTree.codes[index - HLIT - 257].length = lengths.pLengths[index];
	}
	GenerateTree(distanceTree);
}

static int GetLengthExtraBitCount(unsigned short const value)
{
	if (value == 285) return 258;
	if (value <= 264) return 0;

	unsigned int bits = (value - 265) / 4 + 1;

	return bits;
}

static int GetDistanceExtraBitCount(unsigned short const value)
{
	if (value <= 3) return 0;

	unsigned int bits = (value - 4) / 2 + 1;

	return bits;
}

static unsigned int ReadLengthExtraBits(InStream &stream, unsigned int const &value)
{
	if (value == 285)
	{
		return 258;
	}

	int bits = GetLengthExtraBitCount(value);

	// Get base value
	unsigned int base;

	if (bits == 0)
	{
		base = value - 254;
	}
	else
	{
		unsigned int a = (value - 265) % 4;
		unsigned int b = 1 << bits;

		unsigned char start = 0b11111000;
		unsigned char shift = 6 - bits;
		unsigned int c = (unsigned char)(start << shift) >> shift;

		base = a * b + c + 11;
	}

	unsigned short extraBits = stream.ReadBits(bits);

	unsigned int length = base + extraBits;
	return length;
}

static unsigned int ReadDistance(InStream &stream, Tree<DISTANCE_ALPHABET_SIZE> const &distanceTree)
{
	Code code = ReadCode(stream, distanceTree);

	int bits = GetDistanceExtraBitCount(code.value);

	// Read extra bits

	// Get base value
	unsigned int base;

	if (bits == 0)
	{
		base = code.value + 1;
	}
	else
	{
		unsigned int a = (code.value - 4) % 2;
		unsigned int b = 1 << bits;

		unsigned short start = 0b1111111111111100;
		unsigned char shift = 15 - bits;
		unsigned int c = (unsigned short)(start << shift) >> shift;

		base = a * b + c + 5;
	}

	unsigned short extraBits = stream.ReadBits(bits);

	unsigned int distance = base + extraBits;
	return distance;
}

void DecompressStream(InStream &stream, OutStream &outStream,
					  Tree<LITERAL_LENGTH_ALPHABET_SIZE> const &literalLengthTree,
					  Tree<DISTANCE_ALPHABET_SIZE> const &distanceTree)
{
	Code code;
	while (true)
	{
		code = ReadCode(stream, literalLengthTree);
		unsigned int const &value = code.value;

		// End of block
		if (value == 256)
		{
			outStream.SetEnd();
			return;
		}

		// Length, Distance pair
		if (value >= 257)
		{
			unsigned int length = ReadLengthExtraBits(stream, value);
			unsigned int distance = ReadDistance(stream, distanceTree);

			outStream.CopyPrev(length, distance);
		}
		// Literal byte
		else
		{
			outStream.AppendByte(value);
		}
	};
}