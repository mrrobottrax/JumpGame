#pragma once

class InStream;
class OutStream;

#define MAX_CODE_LENGTH 15

// Used for the codelength tree lengths
#define MAX_CODELENGTH_LENGTH 7

#define LITERAL_LENGTH_ALPHABET_SIZE 288
#define DISTANCE_ALPHABET_SIZE 32
#define CODELENGTHS_ALPHABET_SIZE 19

typedef unsigned short code_t;

struct Code
{
	code_t code;
	unsigned short value;
	unsigned char length;
};

template<unsigned int alphabetSize, unsigned char maxCodeLength = MAX_CODE_LENGTH>
struct Tree
{
	Code codes[alphabetSize];
	unsigned int buckets[maxCodeLength + 1];
};

void DecompressStream(InStream &stream, OutStream &outStream,
					  const Tree<LITERAL_LENGTH_ALPHABET_SIZE> &literalLengthTree,
					  const Tree<DISTANCE_ALPHABET_SIZE> &distanceTree);

void DecompressStreamStaticTrees(InStream &stream, OutStream &outStream);

void ReadTrees(InStream &stream,
			   Tree<LITERAL_LENGTH_ALPHABET_SIZE> &literalLengthTree,
			   Tree<DISTANCE_ALPHABET_SIZE> &distanceTree);