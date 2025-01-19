#include "Encryption.hpp"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstring>
#include <stdexcept>

// Encrypt a message using AES-128 in CBC mode
std::string EncryptMessage(const std::string& message, const unsigned char* key)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
    }

    // Generate a random IV
    unsigned char iv[EVP_MAX_IV_LENGTH] = { 0 };
    if (!RAND_bytes(iv, EVP_CIPHER_iv_length(EVP_aes_128_cbc())))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to generate IV");
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("EVP_EncryptInit_ex failed");
    }

    unsigned char encrypted[1024] = { 0 };
    int encryptedLen = 0;

    if (EVP_EncryptUpdate(ctx, encrypted, &encryptedLen,
        (const unsigned char*)message.c_str(), message.size()) != 1)
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

    // Combine IV and ciphertext
    std::string result((char*)iv, EVP_CIPHER_iv_length(EVP_aes_128_cbc()));
    result += std::string((char*)encrypted, encryptedLen + finalLen);
    return result;
}

// Decrypt a message using AES-128 in CBC mode
std::string DecryptMessage(const std::string& encryptedMessage, const unsigned char* key)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
    }

    // Extract IV from the beginning of the message
    const int ivLength = EVP_CIPHER_iv_length(EVP_aes_128_cbc());
    if (encryptedMessage.size() < ivLength)
    {
        throw std::runtime_error("Invalid encrypted message");
    }

    const unsigned char* iv = (const unsigned char*)encryptedMessage.data();
    const unsigned char* ciphertext = (const unsigned char*)encryptedMessage.data() + ivLength;
    const int ciphertextLen = encryptedMessage.size() - ivLength;

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("EVP_DecryptInit_ex failed");
    }

    unsigned char decrypted[1024] = { 0 };
    int decryptedLen = 0;

    if (EVP_DecryptUpdate(ctx, decrypted, &decryptedLen, ciphertext, ciphertextLen) != 1)
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
