#pragma once

#include <unordered_map>;

struct HuffmanNode;

class HuffmanImplementation
{
public:
	HuffmanImplementation();

	void BuildTree(char data[], int freq[], unsigned int size);
	void GenerateCodes(HuffmanNode* node, std::string code);

	void PrintCodes();

private:
	typedef std::unordered_map<char, std::string> huffmanCodeMap;
	huffmanCodeMap m_codeMap; //Store our codes

	HuffmanNode* m_root; //Root node of huffman tree
};

