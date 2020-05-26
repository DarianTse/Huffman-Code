#pragma once

struct HuffmanNode
{
	char m_data;
	unsigned int m_frequency; 
	HuffmanNode* m_left, * m_right;

	bool IsLeaf() { return !(m_left || m_right); }
};

class HuffmanMinHeap
{
public:
	HuffmanMinHeap(unsigned int capacity);

	HuffmanNode* Insert(char data, unsigned int frequency);
	void BuildHeap();
	void MinHeapify(unsigned int index);
	HuffmanNode* ExtractMinNode();
	void SwapNodes(HuffmanNode** a, HuffmanNode** b);

	unsigned int Size() { return m_size; }

	void PrintNodes();

private: 
	unsigned int m_size;
	unsigned int m_capacity;
	HuffmanNode** m_array;
};

