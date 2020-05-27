# Huffman-Code

Implementation of Huffman Coding 

## About

Encode input source symbols into codes of different length, with shorter codes being assigned to more symbols that occur more frequently. 

## Implementation

1. Read input file, collect each character's frequency and store inside a map
2. Create a priority queue implemented with a Min Heap and fill it with the data we stored in the map
3. Build Huffman tree
4. Encode file 
	- 2 parts
		- code map
		- actual data we are encoding
	- For the code map, encoded as : actual char code + (huffman code w/ padding to fill 24 bits). This is 5 bytes for every code stored.
	- For the data, encoded as : base64, converting the actual data into a stream of bits and segmenting it into 6 bit parts, then storing the char representation
	- Also encoding utility data like number of codes at the beginning and padding size at the end
	
5. Decode File
	- Recreate our code map
	- Decode our actual data by matching bit sequences to an entry that exists in the code map. 