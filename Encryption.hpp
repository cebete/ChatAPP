#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <string>

// Encrypt a message using AES
std::string EncryptMessage(const std::string& message, const unsigned char* key);

// Decrypt a message using AES
std::string DecryptMessage(const std::string& encryptedMessage, const unsigned char* key);

#endif // ENCRYPTION_HPP