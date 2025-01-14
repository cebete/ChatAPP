#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <string>

// Encrypt a message using AES
std::string encryptMessage(const std::string& message, const unsigned char* key);

// Decrypt a message using AES
std::string decryptMessage(const std::string& encryptedMessage, const unsigned char* key);

#endif // ENCRYPTION_HPP