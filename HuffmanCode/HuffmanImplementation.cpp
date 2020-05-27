#include "HuffmanImplementation.h"
#include "HuffmanMinHeap.h"

#include <iostream>
#include <fstream>

string encodedFileDir = "../EncodedFiles/";
string encodedOutputFileName = "DefaultEncodedOutputFile.txt";

string decodedFileDir = "../DecodedFiles/";
string decodedOutputFileName = "DefaultDecodedOutputFile.txt";

string originalFileDir = "../OriginalFiles/";

HuffmanImplementation::HuffmanImplementation(string action, string inputFilePath)
	: m_root(nullptr), m_inputFilePath(inputFilePath)
{
	if (action == "Encode")
	{
		BuildTree();
		GenerateCodes(m_root, "");
		PrintCodes();
		EncodeFile();
	}
	else if (action == "Decode")
	{
		DecodeFile();
	}
}

HuffmanMinHeap* HuffmanImplementation::GenerateMinHeapFromInputData()
{
	fstream inputFile;
	inputFile.open(originalFileDir + m_inputFilePath, ios::in);
	if (!inputFile.is_open())
	{
		cout << "Error opening input file \n";
		char c; cin >> c;
		exit(1);
	}

	unordered_map<char, int> charFrequencyMap;

	char c;
	while (inputFile.get(c))
	{
		unordered_map<char, int>::iterator it = charFrequencyMap.find(c);
		if (it == charFrequencyMap.end())
		{
			charFrequencyMap[c] = 1;
		}
		else
		{
			++it->second;
		}
	}
	inputFile.close();

	HuffmanMinHeap* minHeap = new HuffmanMinHeap(charFrequencyMap.size());

	for (auto uniqueEntry : charFrequencyMap)
	{
		minHeap->Insert(uniqueEntry.first, uniqueEntry.second);
	}

	return minHeap;
}

void HuffmanImplementation::EncodeFile()
{
	fstream encodedFile;
	encodedFile.open(encodedFileDir + encodedOutputFileName, ios::out | ios::binary);
	
	fstream inputFile;
	inputFile.open(originalFileDir + m_inputFilePath, ios::in);
	
	string output;
	string stream;

	//Store huffman codes for writing
	output += (char)m_codeMap.size();
	for (auto uniqueEntry : m_codeMap)
	{
		output += uniqueEntry.first;

		//code -> 24bits
		stream.assign(23 - uniqueEntry.second.size(), '0');
		stream += '1'; //Identify beginning of padding
		stream.append(uniqueEntry.second);

		//TODO : consideration when huffman code is greater than 24bits

		//Convert binary to decimal to char representation
		//Split into 4 groups of 6 bits to fit base64 encoding
		for (int i = 0; i < 4; ++i)
		{
			output += (char)BinaryToDecimal(stream.substr(i * 6, 6));
		}
	}
	stream.clear();

	char c;
	while (inputFile.get(c))
	{
		//create a giant binary stream by replacing each character in the file with it's huffman code
		stream += m_codeMap.find(c)->second;
	}

	//convert binary stream to char representation
	int i = 0;
	while (i < (stream.size()/6))
	{
		output += (char)BinaryToDecimal(stream.substr(i++ * 6, 6));
	}

	//add padding to fill up remaining bits
	int remainingBits = stream.size() - (i * 6); 
	int paddingSize = 6 - remainingBits;
	if (paddingSize > 0)
	{
		stream = stream.substr(i * 6);
		stream.append(paddingSize, '0');
		output += (char)BinaryToDecimal(stream);

		//used to strip off the padding when we decode
		output += (char)paddingSize;
	}
	
	encodedFile.write(output.c_str(), output.size());

	encodedFile.close();
	inputFile.close();
}

void HuffmanImplementation::DecodeFile()
{
	fstream decodedFile;
	decodedFile.open(decodedFileDir + decodedOutputFileName, ios::out);

	//Decode our huffman codes
	fstream inputFile;
	inputFile.open(encodedFileDir + m_inputFilePath, ios::in | ios::binary);

	//First char is the number of codes
	char numCodes;
	inputFile.read(&numCodes, 1);

	//char symbol followed by 24bits huffman code represented in 4 chars - total 5 char stride
	for (int i = 0; i < numCodes; ++i)
	{
		char* codeStride = new char[5];
		inputFile.read(codeStride, 5);

		//Our char symbol is at codeStride[0]
		//Decode char to 24 bit segment
		string twentyFourBitSegment;
		for (int j = 1; j < 5; ++j)
		{
			cout << "-" << codeStride[j] << endl;
			twentyFourBitSegment += DecimalToBinary(codeStride[j]);
		}

		//Filter the padding to get the huffman code
		int pos = twentyFourBitSegment.find_first_of('1');
		string huffmanCode = twentyFourBitSegment.substr(pos + 1);

		cout << codeStride[0] <<  huffmanCode << endl;
		m_decodeMap[huffmanCode] = codeStride[0];
	}

	//Go through the content and convert chars into stream of bits
	string stream;
	char c;
	while (inputFile.get(c))
	{
		stream += DecimalToBinary(c);
	}

	//last char (8bits) are the end padding size
	char endPaddingSize = (char)BinaryToDecimal(stream.substr(stream.size() - 8, 8));
	stream = stream.substr(0, stream.size() - 8);

	//convert stream of bits into original data based on huffman codes
	auto min = [](int a, int b)
	{
		return a > b ? b : a;
	};
	
	int smallestCodeSize = INT_MAX;
	for (auto uniqueEntry : m_decodeMap)
	{
		smallestCodeSize = min(smallestCodeSize, uniqueEntry.first.size());
	}

	huffmanDecodeMap::iterator it;
	string code = "";
	string output = "";
	for (int i = 0; i < stream.size(); ++i)
	{
		code += stream[i];
		if (code.size() >= smallestCodeSize)
		{
			it = m_decodeMap.find(code);
			if (it != m_decodeMap.end())
			{
				output += it->second;
				code.clear();
			}
		}
	}

	decodedFile.write(output.c_str(), output.size());

	inputFile.close();
	decodedFile.close();
}

void HuffmanImplementation::BuildTree()
{
	HuffmanMinHeap* minHeap = GenerateMinHeapFromInputData();
	minHeap->BuildHeap();
	minHeap->PrintNodes();

	HuffmanNode* left, * right, * parent = nullptr;

	while (minHeap->Size() != 1)
	{
		//remove 2 smallest frequency nodes, and create a new node with the sum of both frequencies 
		left = minHeap->ExtractMinNode();
		right = minHeap->ExtractMinNode();

		parent = minHeap->Insert('~', left->m_frequency + right->m_frequency);
		parent->m_left = left;
		parent->m_right = right;
	}

	m_root = parent;
}

//Iterate thorugh our min heap to generate our codes and fill our code map
void HuffmanImplementation::GenerateCodes(HuffmanNode* node, std::string code)
{
	if (!node)
	{
		return;
	}

	if (node->IsLeaf())
	{
		m_codeMap[node->m_data] = code;
	}
	else
	{
		GenerateCodes(node->m_left, code + '0');
		GenerateCodes(node->m_right, code + '1');
	}
}

void HuffmanImplementation::PrintCodes()
{
	for (auto uniqueEntry : m_codeMap)
	{
		cout << uniqueEntry.first << " : " << uniqueEntry.second << "(" <<  uniqueEntry.second.size() << ") \n";
	}
}

int HuffmanImplementation::BinaryToDecimal(string n)
{
	int base = n.size() - 1;

	int i = 0;
	for (auto c : n)
	{
		i += pow(2, base--) * (c - '0');
	}
	return i;
}

string HuffmanImplementation::DecimalToBinary(int n)
{
	auto swap = [](char& a, char& b)
	{
		char temp = move(a);
		a = move(b);
		b = move(temp);
	};

	string result = "";
	while (n > 0)
	{
		result += (n % 2) ? '1' : '0';
		n = n / 2;
	}

	result.append(6 - result.size(), '0');

	for (int j = 0; j < result.size() / 2; ++j)
	{
		swap(result[j], result[result.size() - j - 1]);
	}

	return result;
}