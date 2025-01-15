#ifndef KEYGENERATION_HPP
#define KEYGENERATION_HPP

// Generate a random key of the specified length
void GenerateRandomKey(unsigned char* key, int length);

// Print the given key in hexadecimal format
void PrintKey(const unsigned char* key, int length);

#endif