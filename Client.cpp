#include "Client.hpp"
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
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

    char buffer[1024] = { 0 };
    while (true)
    {
        std::string message;

        // Prompt the user for input
        std::cout << "You: ";
        std::getline(std::cin, message);

        // Skip if the message is empty
        if (message.empty())
        {
            continue;
        }

        // Send the message to the server
        if (send(clientSocket, message.c_str(), message.size(), 0) == SOCKET_ERROR)
        {
            std::cerr << "Failed to send message to server!" << std::endl;
            break;
        }

        // Wait for the server's response
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0)
        {
            std::cout << "Server disconnected!" << std::endl;
            break;
        }

        // Print the server's response
        std::cout << "Server: " << buffer << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();
}

