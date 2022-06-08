// MapReduce-SocketServer.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <filesystem>
#include <thread>
#include <string>
#include "Utilities.h"

using FuncMap = void (*)(const std::vector<std::filesystem::path>& filePaths, const std::filesystem::path& tempOutputPath);
using FuncReduce = void (*)(const std::vector<std::filesystem::path>& filePaths, int totalFileCount, const std::filesystem::path& outputFilePath);

// Globals
const std::filesystem::path inputDirectory = "..\\CSE-687_OOD-Project\\inputfiles";
const std::filesystem::path tempDirectory =  "..\\CSE-687_OOD-Project\\tempfiles";
const std::filesystem::path outputDirectory = "..\\CSE-687_OOD-Project\\outputfiles";
const wchar_t* mapReduceDllPath = L"MapReduceLibrary.dll";
FuncMap Map;
FuncReduce Reduce;


int MapperPartition(const std::vector<std::filesystem::path>& inputFiles, FuncMap map, const int numMapperThreads = 5);
void ReducerPartition(const std::vector<std::filesystem::path>& tempFiles, FuncReduce reduce, int numOfReducerThreads = 5);
void StartTasks(const std::vector<std::filesystem::path>& files, int numThreads, FuncMap map, FuncReduce reduce);
void error(const std::string& message);

int main(int argc, char* argv[])
{
    // load dll library for use later
    HINSTANCE mapReduceLibraryHandle;
    mapReduceLibraryHandle = LoadLibraryEx(mapReduceDllPath, nullptr, NULL);

    if (mapReduceLibraryHandle != nullptr) {

        Map = (FuncMap)GetProcAddress(mapReduceLibraryHandle, "Map");
        Reduce = (FuncReduce)GetProcAddress(mapReduceLibraryHandle, "Reduce");
    }

    // WSA Startup init
    WSAData wsa_data = {};
    if (long wsa_status = WSAStartup(MAKEWORD(2, 2), &wsa_data); wsa_status != 0) {
        error("error initializing WSA Startup");
    }

    // address for both the listening and connection socket
    int portNumber = std::stoi(argv[1]);
    sockaddr_in address;
    int address_size = sizeof(address);

    // sockets
    SOCKET socket_listen;
    SOCKET socket_connection;

    // socket formatting
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
    address.sin_family = AF_INET;
    address.sin_port = htons(portNumber);

    // listen socket
    socket_listen = socket(AF_INET, SOCK_STREAM, NULL);
    if (socket_listen == -1) {
        error("listen socket failed");
    }

    // bind the listen socket
    if (int bind_status = bind(socket_listen, (sockaddr*)&address, sizeof(address)); bind_status == -1) {
        std::cout << WSAGetLastError() << std::endl;
        error("binding failed");
    }

    // listen for incoming connections
    listen(socket_listen, 10);

    std::cout << "Waiting for incoming connection on socket " << socket_listen << "..." << std::endl;

    if ((socket_connection = accept(socket_listen, (sockaddr*)&address, &address_size)) != -1) {
        std::cout << "Successful connection with client socket: " << socket_connection << std::endl;
        char buffer[] = "Connection with MapReduce Server successful";
        send(socket_connection, buffer, sizeof(buffer), NULL);
    }

    while (true) {
        char buffer[1024];
        int value = recv(socket_connection, buffer, sizeof(buffer), NULL);
        if (value != -1) {
            std::string response = buffer;
            if (response == "map") {
                std::vector<std::filesystem::path> inputFiles = Utilities::GetFilesInDirectory(inputDirectory);
                MapperPartition(inputFiles, Map);
                // update the client that the job is finished
                char responsebuffer[10] = "done";
                send(socket_connection, responsebuffer, sizeof(responsebuffer), NULL);
            }
            else if (response == "reduce") {
                std::vector<std::filesystem::path> tempFiles = Utilities::GetFilesInDirectory(tempDirectory);
                ReducerPartition(tempFiles, Reduce);
                // update the client that the job is finished
                char responsebuffer[10] = "done";
                send(socket_connection, responsebuffer, sizeof(responsebuffer), NULL);
            }
            else if (response == "stop") {
                break;
            }
        }
    }

    closesocket(socket_connection);
    closesocket(socket_listen);
    WSACleanup();
}

int MapperPartition(const std::vector<std::filesystem::path>& inputFiles, FuncMap map, const int numMapperThreads) {

    StartTasks(inputFiles, numMapperThreads, map, nullptr);
    return numMapperThreads;
}

void ReducerPartition(const std::vector<std::filesystem::path>& tempFiles, FuncReduce reduce, int numOfReducerThreads) {
    StartTasks(tempFiles, numOfReducerThreads, nullptr, reduce);
}

void StartTasks(const std::vector<std::filesystem::path>& files, int numThreads, FuncMap map, FuncReduce reduce) {
    // bucket the files based on num of threads
    std::vector<std::thread> threads;

    // calculation to determine how many jobs will be given to each thread
    auto jobsPerThread = (files.size() / numThreads) + (files.size() % numThreads != 0);

    for (int i = 0; i < numThreads; i++) {
        std::vector<std::filesystem::path> threadFiles;

        for (auto j = jobsPerThread * i; j < jobsPerThread * i + jobsPerThread; j++) {

            if (j >= files.size())
            {
                break;
            }

            if (map != nullptr) {

                std::cout << "Mapping " << files[j].filename() << std::endl;
            }
            else {
                std::cout << "Reducing " << files[j].filename() << std::endl;
            }

            threadFiles.push_back(files[j]);
        }
        // Pass the list of files for the map function (can either send vector of files, or make another 'for-loop' to go through each file in each vector
        if (map != nullptr) {
            threads.push_back(std::thread(map, threadFiles, tempDirectory));
        }
        else {
            threads.push_back(std::thread(reduce, threadFiles, files.size(), outputDirectory));
        }
    }

    // wait until each mapperThread finishes processing their files
    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

void error(const std::string& message) {
    std::cout << message << std::endl;
    exit(1);
}