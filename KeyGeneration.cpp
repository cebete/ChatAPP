#include "KeyGeneration.hpp"
#include <openssl/rand.h>
#include <iostream>
#include <stdexcept>

// Generate a random key of the specified length
void GenerateRandomKey(unsigned char* key, int length)
{
	if (RAND_bytes(key, length) != 1) 
	{
		throw std::runtime_error("Failed to generate random key");
	}
}

// Print the given key in hexadecimal format
void PrintKey(const unsigned char* key, int length)
{
	std::cout << "Generated Key (hex): ";
	for (int i = 0; i < length; ++i)
	{
		printf("%02x ", key[i]);
	}
	std::cout << std::endl;
}