#include "Server.hpp"
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

void ServerFunction()
{
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

        std::cout << "Client connected!" << std::endl;

        char buffer[1024] = { 0 };
        while (true)
        {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

            if (bytesRead == 0)
            {
                std::cout << "Client disconnected!" << std::endl;
                break;
            }
            else if (bytesRead == SOCKET_ERROR)
            {
                int errorCode = WSAGetLastError();
                if (errorCode == WSAECONNRESET)
                {
                    std::cout << "Client disconnected unexpectedly!" << std::endl;
                }
                else
                {
                    std::cerr << "Recv failed, error: " << errorCode << std::endl;
                }
                break;
            }

            std::cout << "Client: " << buffer << std::endl;

            std::string reply;
            std::cout << "You: ";
            std::getline(std::cin, reply);
            send(clientSocket, reply.c_str(), reply.size(), 0);
        }

        closesocket(clientSocket);
        std::cout << "Waiting for a new connection..." << std::endl;
    }

    closesocket(serverSocket);
    WSACleanup();
}