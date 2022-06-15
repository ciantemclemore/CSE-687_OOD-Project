#include "Workflow.h"

/// <summary>
/// Constructor to initialize the workflow component
/// </summary>
Workflow::Workflow(const std::filesystem::path& inputDir, const std::filesystem::path& tempDir, const std::filesystem::path& outputDir, const wchar_t* dllPath)
	: inputDirectory(inputDir), tempDirectory(tempDir), outputDirectory(outputDir), mapReduceDllPath(dllPath) {
}

void Workflow::Init() const {
	
	// create each server process and store the process information for later cleanup
	std::vector<PROCESS_INFORMATION> process_informations = CreateServerProcesses();

	// WSA Startup initialization to use winsocks
	WSAData wsa_data = {};
	if (long wsa_status = WSAStartup(MAKEWORD(2, 2), &wsa_data); wsa_status != 0) {
		Error("error initializing WSA Startup");
	}

	// server socket collection
	std::vector<SOCKET> server_sockets = ConnectClients();
	std::vector<std::filesystem::path> inputFiles = Utilities::GetFilesInDirectory(inputDirectory);
	
	// send work to the first server client to handle mapping
	// this buffer will contain the "map" message to trigger Map
	char mapBuffer[] = "map";
	send(server_sockets[0], mapBuffer, sizeof(mapBuffer), NULL);

	// Have the client wait until the mapping process is complete
	// Save the map response in the buffer and check to finish
	char mapResponseBuffer[10];
	while (true) {
		int value =recv(server_sockets[0], mapResponseBuffer, sizeof(mapResponseBuffer), NULL);
		if (value != -1) {
			std::string response = mapResponseBuffer;
			if (response == "done") {
				break;
			}
		}
	}

	// now the other process can begin reducing
	char reduceBuffer[] = "reduce";
	send(server_sockets[1], reduceBuffer, sizeof(reduceBuffer), NULL);

	// Have the client wait until the reducing process is complete
	// Sace the reduce response in the buffer and check to finish
	char reduceResponseBuffer[10];
	while (true) {
		int value = recv(server_sockets[1], reduceResponseBuffer, sizeof(reduceResponseBuffer), NULL);
		if (value != -1) {
			std::string response = reduceResponseBuffer;
			if (response == "done") {
				break;
			}
		}
	}

	// Begin clean up process for the sockets and processes
	SocketCleanup(server_sockets);
	ProcessCleanup(process_informations);
}

std::vector<PROCESS_INFORMATION> Workflow::CreateServerProcesses() const {
	
	std::vector<PROCESS_INFORMATION> process_informations;

	for (int i = 0; i < 2; i++) {
		STARTUPINFO si = {};
		PROCESS_INFORMATION pi = {};

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		int portNumber = BASEPORT + i;
		std::string applicationName = "..\\x64\\Debug\\MapReduce-SocketServer.exe";
		std::string command_arg_string = applicationName.append(" ").append(std::to_string(portNumber));
		wchar_t command_args[60] = {};

		// copy the command line argument string to the LPWSTR
		for (int j = 0; j < command_arg_string.size(); j++)
		{
			command_args[j] = command_arg_string[j];
		}

		if (!CreateProcess(NULL, command_args, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
			std::cout << "Create Process failed: " << GetLastError() << std::endl;
			exit(1);
		}

		// store the process information to close the handle on the process & thread later
		process_informations.emplace_back(pi);
	}

	return process_informations;
}

void Workflow::ProcessCleanup(const std::vector<PROCESS_INFORMATION>& proc_informations) const {
	// close all the handles and threads for each process
	for(const auto& pi : proc_informations) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

void Workflow::SocketCleanup(const std::vector<SOCKET>& server_sockets) const {
	// close down the sockets on the client application
	for (const auto& socket : server_sockets) {
		// now we can alert the server processes to stop and cleanup resources
		char stopBuffer[] = "stop";
		send(socket, stopBuffer, sizeof(stopBuffer), NULL);
		closesocket(socket);
	}
	WSACleanup();
}

std::vector<SOCKET> Workflow::ConnectClients() const {
	std::vector<SOCKET> server_sockets;

	// first connect each client to the server
	for (unsigned short int i = 0; i < 2; i++) {

		char buffer[1024] = {};
		std::string server_message;

		// create a socket to connect to the server
		SOCKET server_socket;
		if ((server_socket = socket(AF_INET, SOCK_STREAM, NULL)) == -1) {
			Error("error opening the socket");
		}

		unsigned short int portNumber = BASEPORT + i;

		// define the server address that the client wants to connect to
		sockaddr_in address;
		inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
		address.sin_family = AF_INET;
		address.sin_port = htons(portNumber);

		// try connecting to the server
		if (int connection_status = connect(server_socket, (struct sockaddr*)&address, sizeof(address)); connection_status == -1) {
			std::cout << WSAGetLastError() << std::endl;
			Error("error connecting to the server socekt");
		}

		recv(server_socket, buffer, sizeof(buffer), NULL);
		server_message = buffer;
		std::cout << server_message << std::endl;

		server_sockets.emplace_back(server_socket);
	}

	return server_sockets;
}

void Workflow::Error(const std::string& message) const {
	std::cout << message << std::endl;
	exit(1);
}
