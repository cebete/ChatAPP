#include "Client.hpp"
#include "Encryption.hpp" // Include the encryption module
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "KeyGeneration.hpp"
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080



void ClientFunction(const std::string& ipAddress)
{
	PCSTR ipAddressPCSTR = ipAddress.c_str();

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "WSAStartup failed!" << std::endl;
		return;
	}

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "Socket creation failed!" << std::endl;
		WSACleanup();
		return;
	}

	sockaddr_in serverAddr = {};
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, ipAddressPCSTR, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(PORT);

	if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Connection to server failed!" << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

	std::cout << "Connected to server!" << std::endl;

	unsigned char key[16];
	if (recv(clientSocket, (char*)key, sizeof(key), 0) <= 0)
	{
		std::cerr << "Failed to receive the encryption key from the server!" << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

	// Display the received key for debugging
	PrintKey(key, sizeof(key));

	char buffer[1024] = { 0 };
	while (true)
	{
		std::string message;

		// Prompt the user for input
		std::cout << "You: ";
		std::getline(std::cin, message);

		if (message.empty())
		{
			continue;
		}

		// Encrypt the message
		std::string encryptedMessage = encryptMessage(message, key);

		// Send the encrypted message
		if (send(clientSocket, encryptedMessage.c_str(), encryptedMessage.size(), 0) == SOCKET_ERROR)
		{
			std::cerr << "Failed to send message to server!" << std::endl;
			break;
		}

		// Receive encrypted data from the server
		memset(buffer, 0, sizeof(buffer));
		int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesRead <= 0)
		{
			std::cout << "Server disconnected!" << std::endl;
			break;
		}

		// Print the raw encrypted data received from the server
		std::cout << "Received Encrypted (hex): ";
		for (int i = 0; i < bytesRead; ++i)
		{
			printf("%02x ", (unsigned char)buffer[i]);
		}
		std::cout << std::endl;

		// Decrypt the server's response
		std::string decryptedResponse = decryptMessage(std::string(buffer, bytesRead), key);

		// Print the decrypted message
		std::cout << "Server (Decrypted): " << decryptedResponse << std::endl;
	}

	closesocket(clientSocket);
	WSACleanup();
}
