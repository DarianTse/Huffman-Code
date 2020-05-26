#include "HuffmanMinHeap.h"

#include <iostream>
#include <cstdlib>
#include <utility>

HuffmanMinHeap::HuffmanMinHeap(unsigned int capacity)
	: m_size(0), m_capacity(capacity)
{
	m_array = new HuffmanNode*[m_capacity];
}

HuffmanNode* HuffmanMinHeap::Insert(char data, unsigned int frequency)
{
	if (m_size == m_capacity)
	{
		return nullptr;
	}

	++m_size;

	HuffmanNode* newNode = new HuffmanNode;
	newNode->m_data = data;
	newNode->m_frequency = frequency;
	newNode->m_left = newNode->m_right = nullptr;

	m_array[m_size - 1] = newNode;

	return newNode;
}

//Loop through non leaf indices, and swap values so that the tree is ordered from small -> large frequencies
void HuffmanMinHeap::BuildHeap()
{
	for (int i = (m_size - 2) / 2; i >= 0; --i)
	{
		MinHeapify(i);
	}
}

//Swap values between index node and child nodes such that the smaller frequency will be in a lower index
void HuffmanMinHeap::MinHeapify(unsigned int index)
{
	unsigned int smallest = index;
	unsigned int left = index * 2 + 1;
	unsigned int right = index * 2 + 2;

	if (left < m_size && m_array[left]->m_frequency < m_array[smallest]->m_frequency)
	{
		smallest = left;
	}

	if (right < m_size && m_array[right]->m_frequency < m_array[smallest]->m_frequency)
	{
		smallest = right;
	}

	if (smallest != index)
	{
		SwapNodes(&m_array[smallest], &m_array[index]);
		MinHeapify(smallest);
	}
}

//Remove smallest element from tree by moving the largest value to the root. Then sort that element back into place.
HuffmanNode* HuffmanMinHeap::ExtractMinNode()
{
	HuffmanNode* min = m_array[0];
	m_array[0] = m_array[m_size - 1];
	--m_size;
	MinHeapify(0);

	return min;
}

void HuffmanMinHeap::SwapNodes(HuffmanNode** a, HuffmanNode** b)
{
	HuffmanNode* temp(std::move(*a));
	*a = std::move(*b);
	*b = std::move(temp);
}

void HuffmanMinHeap::PrintNodes()
{
	for (unsigned int i = 0; i < m_size; ++i)
	{
		std::cout << m_array[i]->m_data << " - ";
	}
	std::cout << "\n";
}