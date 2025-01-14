#include "Server.hpp"
#include "Client.hpp"
#include <iostream>
#include <string>

int main()
{
    std::cout << "Run as server or client? (s/c): ";
    char choice;
    std::cin >> choice;

    // Clear the input buffer to avoid leftover newline characters
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 's' || choice == 'S')
    {
        ServerFunction();
    }
    else if (choice == 'c' || choice == 'C')
    {
        std::string ipAddress;
        std::cout << "Enter IP address to connect to: ";
        std::getline(std::cin, ipAddress); // Use getline to ensure proper input handling
        ClientFunction(ipAddress);
    }
    else
    {
        std::cerr << "Invalid choice! Please restart the program and select 's' for server or 'c' for client." << std::endl;
    }

    return 0;
}
