#include "Encryption.hpp"
#include <openssl/evp.h>
#include <cstring>
#include <stdexcept>

// Encrypt a message using AES-128 in ECB mode
std::string EncryptMessage(const std::string& message, const unsigned char* key) 
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	if (!ctx) 
	{
		throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
	}

	if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, key, nullptr) != 1) 
	{
		EVP_CIPHER_CTX_free(ctx);
		throw std::runtime_error("EVP_EncryptInit_ex failed");
	}

	unsigned char encrypted[128] = { 0 };
	int encryptedLen = 0;

	if (EVP_EncryptUpdate(ctx, encrypted, &encryptedLen, (const unsigned char*)message.c_str(), message.size()) != 1) 
	{
		EVP_CIPHER_CTX_free(ctx);
		throw std::runtime_error("EVP_EncryptUpdate failed");
	}

	int finalLen = 0;
	if (EVP_EncryptFinal_ex(ctx, encrypted + encryptedLen, &finalLen) != 1) 
	{
		EVP_CIPHER_CTX_free(ctx);
		throw std::runtime_error("EVP_EncryptFinal_ex failed");
	}

	EVP_CIPHER_CTX_free(ctx);

	return std::string((char*)encrypted, encryptedLen + finalLen);
}

// Decrypt a message using AES-128 in ECB mode
std::string DecryptMessage(const std::string& encryptedMessage, const unsigned char* key) 
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	if (!ctx) 
	{
		throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
	}

	if (EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, key, nullptr) != 1) 
	{
		EVP_CIPHER_CTX_free(ctx);
		throw std::runtime_error("EVP_DecryptInit_ex failed");
	}

	unsigned char decrypted[128] = { 0 };
	int decryptedLen = 0;

	if (EVP_DecryptUpdate(ctx, decrypted, &decryptedLen, (const unsigned char*)encryptedMessage.c_str(), encryptedMessage.size()) != 1) 
	{
		EVP_CIPHER_CTX_free(ctx);
		throw std::runtime_error("EVP_DecryptUpdate failed");
	}

	int finalLen = 0;
	if (EVP_DecryptFinal_ex(ctx, decrypted + decryptedLen, &finalLen) != 1) 
	{
		EVP_CIPHER_CTX_free(ctx);
		throw std::runtime_error("EVP_DecryptFinal_ex failed");
	}

	EVP_CIPHER_CTX_free(ctx);

	return std::string((char*)decrypted, decryptedLen + finalLen);
}
