#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

/**
 * @brief connects to a TCP server at the given IP address and facilitates message exchange between the client and server.
 *
 *
 * @param  ipAdress:  A [std::string] containing the IPv4 address of the server to connect to (e.g., "127.0.0.1").
 */
void ClientFunction(const std::string& ipAdress);


#endif