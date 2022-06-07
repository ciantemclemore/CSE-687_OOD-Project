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

    sockaddr_in address;
    int address_size = sizeof(address);

    // sockets
    SOCKET socket_listen;
    SOCKET socket_connection;

    // socket creation
  /*  socket_connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (socket_connection == -1) {
        error("connect socket failed");
    }*/

    // socket formatting
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    // listen socket
    socket_listen = socket(AF_INET, SOCK_STREAM, NULL);
    if (socket_listen == -1) {
        error("listen socket failed");
    }

    // bind the listen socket
    if (int bind_status = bind(socket_listen, (sockaddr*)&address, sizeof(address)); bind_status == -1) {
        error("binding failed");
    }

    // listen for incoming connections
    listen(socket_listen, 10);

    for (;;) {
        std::cout << "Waiting for incoming connection..." << std::endl;
        if ((socket_connection = accept(socket_listen, (sockaddr*)&address, &address_size)) != -1) {
            std::cout << "successful connection to " << socket_connection << std::endl;
            std::string welcome_message = "You are now connected";
            send(socket_connection, welcome_message.c_str(), sizeof(welcome_message), NULL);
        }
    }
   
}

void error(const std::string& message) {
    std::cout << message << std::endl;
    exit(1);
}