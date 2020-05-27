#include "HuffmanImplementation.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	string inputFile;
	string action;

	if (argc < 3)
	{
		cout << "Requires arguments 1)[Encode/Decode] 2) input file \n";
		cout << "To encode, provide original source file - To decode, provide encoded file \n";
		cout << "Using default files \n";
	
		//For testing 

	    action = "Encode";
		//action = "Decode";

		if (action == "Decode")
		{
			inputFile = encodedOutputFileName;
		}
		else
		{
			inputFile = "TestInputFile.txt";
		}
	}
	else
	{
		action = argv[1];
		inputFile = argv[2];
	}

	HuffmanImplementation* huffman = new HuffmanImplementation(action, inputFile);
	delete(huffman);
	return 0;
}