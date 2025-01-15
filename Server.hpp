#ifndef SERVER_HPP
#define SERVER_HPP

/**
 * @brief Starts a TCP server, listens on a specified port, handles client connections, and facilitates message exchange until the client disconnects. It continues running to accept new connections. 
 */
void ServerFunction();
void GenerateRandomKey(unsigned char key[16]);
void printKey(const unsigned char key[16]);

#endif