#include "HuffmanImplementation.h"
#include "HuffmanMinHeap.h"

#include <iostream>

HuffmanImplementation::HuffmanImplementation()
	: m_root(nullptr)
{
	char data[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	int frequency[] = { 6, 5, 4, 3, 2, 1 };
	BuildTree(data, frequency, sizeof(data)/sizeof(char));
	GenerateCodes(m_root, "");
	PrintCodes();
}

void HuffmanImplementation::BuildTree(char data[], int freq[], unsigned int size)
{
	HuffmanNode* left, * right, * parent = nullptr;

	HuffmanMinHeap* minHeap = new HuffmanMinHeap(size);
	for (int i = 0; i < size; ++i)
	{
		minHeap->Insert(data[i], freq[i]);
	}

	minHeap->BuildHeap();
	minHeap->PrintNodes();

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
	huffmanCodeMap::iterator it;
	for (it = m_codeMap.begin(); it != m_codeMap.end(); ++it)
	{
		std::cout << it->first << " : " << it->second << "\n";
	}
}
