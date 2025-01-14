#include "Server.hpp"
#include "Encryption.hpp" // Include the encryption module
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

void ServerFunction() {
    const unsigned char key[16] = { '1', '2', '3', '4', '5', '6', '7', '8',
                                   '9', '0', '1', '2', '3', '4', '5', '6' };

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        std::cerr << "Listen failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Client connection failed!" << std::endl;
            continue;
        }

        std::cout << "Client connected!" << std::endl;

        char buffer[1024] = { 0 };
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

            if (bytesRead <= 0) {
                std::cout << "Client disconnected!" << std::endl;
                break;
            }

            // Print the raw encrypted data received
            std::cout << "Encrypted Received (hex): ";
            for (int i = 0; i < bytesRead; ++i) {
                printf("%02x ", (unsigned char)buffer[i]);
            }
            std::cout << std::endl;

            // Decrypt the received message
            std::string decryptedMessage = decryptMessage(std::string(buffer, bytesRead), key);
            std::cout << "Decrypted: " << decryptedMessage << std::endl;

            // Encrypt the server's response
            std::string reply;
            std::cout << "You: ";
            std::getline(std::cin, reply);

            std::string encryptedReply = encryptMessage(reply, key);
            send(clientSocket, encryptedReply.c_str(), encryptedReply.size(), 0);
        }

        closesocket(clientSocket);
        std::cout << "Waiting for a new connection..." << std::endl;
    }

    closesocket(serverSocket);
    WSACleanup();
}
