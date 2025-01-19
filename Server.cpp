#include "Server.hpp"
#include "Encryption.hpp" // Include the encryption module
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "KeyGeneration.hpp"
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

void ServerFunction()
{
    unsigned char key[16];

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed!" << std::endl;
        return;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    if (listen(serverSocket, 3) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true)
    {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Client connection failed!" << std::endl;
            continue;
        }

        GenerateRandomKey(key, sizeof(key));
        PrintKey(key, sizeof(key)); // FOR DEBUGGING ONLY

        // Send the key to the client
        if (send(clientSocket, (const char*)key, sizeof(key), 0) == SOCKET_ERROR)
        {
            std::cerr << "Failed to send the encryption key to the client!" << std::endl;
            closesocket(clientSocket);
            continue;
        }

        std::cout << "Client connected!" << std::endl;

        char buffer[2048] = { 0 };
        while (true)
        {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

            if (bytesRead <= 0)
            {
                std::cout << "Client disconnected!" << std::endl;
                break;
            }

            std::cout << "Encrypted Received (hex): ";
            for (int i = 0; i < bytesRead; ++i)
            {
                printf("%02x ", (unsigned char)buffer[i]);
            }
            std::cout << std::endl;

            try
            {
                // Decrypt the received message
                std::string decryptedMessage = DecryptMessage(std::string(buffer, bytesRead), key);
                std::cout << "Decrypted: " << decryptedMessage << std::endl;

                // Encrypt the server's response
                std::string reply;
                std::cout << "You: ";
                std::getline(std::cin, reply);

                std::string encryptedReply = EncryptMessage(reply, key);
                send(clientSocket, encryptedReply.c_str(), encryptedReply.size(), 0);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

        closesocket(clientSocket);
        std::cout << "Waiting for a new connection..." << std::endl;
    }

    closesocket(serverSocket);
    WSACleanup();
}
