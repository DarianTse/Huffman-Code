#pragma once

#include <unordered_map>
#include <string>

using namespace std;

struct HuffmanNode;
class HuffmanMinHeap;

extern string encodedFileDir;
extern string encodedOutputFileName;

extern string decodedFileDir;
extern string decodedOutputFileName;

extern string originalFileDir;

class HuffmanImplementation
{
public:
	HuffmanImplementation(string action, string inputFilePath);

private: 
	HuffmanMinHeap* GenerateMinHeapFromInputData();
	void EncodeFile();
	void DecodeFile();

	void BuildTree();
	void GenerateCodes(HuffmanNode* node, string code);
	void PrintCodes();

	int BinaryToDecimal(string n);
	string DecimalToBinary(int n);

private:
	typedef unordered_map<char, string> huffmanCodeMap;
	huffmanCodeMap m_codeMap; //Store our codes

	typedef unordered_map<string, char> huffmanDecodeMap;
	huffmanDecodeMap m_decodeMap;

	HuffmanNode* m_root; //Root node of huffman tree

	string m_inputFilePath;
};
