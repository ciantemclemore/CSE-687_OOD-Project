// MapReduce-SocketServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <string>
#include <WS2tcpip.h>


void error(const std::string& message);

int main()
{
    // WSA Startup init
    WSAData wsa_data = {};
    if (long wsa_status = WSAStartup(MAKEWORD(2, 2), &wsa_data); wsa_status != 0) {
        error("error initializing WSA Startup");
    }

    // create the server socket
    size_t server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, NULL);
    if (server_socket == -1) {
        error("error opening the socket");
    }

    // specify the server address
    struct sockaddr_in address = {};
    socklen_t address_size = sizeof address;
    address.sin_family = AF_INET;
    address.sin_port = htons(5500);
    address.sin_addr.s_addr = INADDR_ANY;
    
    // bind the socket to an IP and PORT
    // also check if binding is successful
    if (int bind_status = bind(server_socket, (struct sockaddr*)&address, address_size); bind_status == -1) {
        std::cout << WSAGetLastError() << std::endl;
        error("error binding the socket");
    }


    // set the socket to listen for incoming connections
    // 10 - the backlog for the socket.. defines the total number of connections able to be queued
    listen(server_socket, 10);

    // the server is now listening for incoming connections
    std::cout << "Listening for incoming connections..." << std::endl;

    // once client connects, save the file descriptor for the successful connection
    size_t client_socket;

    for (;;) {
        if (client_socket = accept(server_socket, (struct sockaddr*)&address, &address_size)) {
            std::cout << "Received connection from " << inet_ntop(AF_INET, &address.sin_addr, nullptr, 0) << ":" << ntohs(address.sin_port) << std::endl;
        }
        else {
            error("error accepting client connection");
        }
    }

  /*  std::string clientMessage;

    while (true) {
        recv(client_socket, clientMessage.data(), sizeof(clientMessage), 0);

        if (clientMessage.compare("quit") == 0) {
            break;
        }
    }

    closesocket(client_socket);
    closesocket(server_socket);*/
}

void error(const std::string& message) {
    std::cout << message << std::endl;
    exit(1);
}