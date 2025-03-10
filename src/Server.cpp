#include "Server.h"
#include "PyCaller.h"
#include <iostream>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    // 定义一个关闭套接字的宏
    #define CLOSE_SOCKET(s) closesocket(s)
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #define CLOSE_SOCKET(s) close(s)
#endif

Server::Server(int port, size_t threadCount) : port(port), threadPool(threadCount) {
#ifdef _WIN32
    // Windows 下需要初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        exit(EXIT_FAILURE);
    }
#endif

    if (!initSocket()) {
        std::cerr << "Failed to initialize socket." << std::endl;
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    CLOSE_SOCKET(server_fd);
#ifdef _WIN32
    WSACleanup();
#endif
}

bool Server::initSocket() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return false;
    }
    
    int opt = 1;
#ifdef _WIN32
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt)) != 0) {
#else
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
#endif
    std::cerr << "setsockopt failed" << std::endl;
    return false;
}
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return false;
    }
    
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return false;
    }
    std::cout << "Server listening on port " << port << std::endl;
    return true;
}

void Server::handleClient(int client_socket) {
    char buffer[1024] = {0};
    int valread = recv(client_socket, buffer, 1024, 0);
    
    if(valread <= 0) {
        std::cerr << "Error: Failed to receive data or client disconnected." << std::endl;
        CLOSE_SOCKET(client_socket);
        return;
    }

    std::string imagePath(buffer);
    std::cout << "Received image path: " << imagePath << std::endl;

    // 调用 Python 进行图像处理
    std::string result = process_image(imagePath);
    if (result.empty()) {
        std::cerr << "Error: Image processing failed!" << std::endl;
        CLOSE_SOCKET(client_socket);
        return;
    }

    std::cout << "Processed image path: " << result << std::endl;
    
    send(client_socket, result.c_str(), result.length(), 0);
    CLOSE_SOCKET(client_socket);
}


void Server::run() {
    while (true) {
        struct sockaddr_in client_address;
        socklen_t addrlen = sizeof(client_address);
        int client_socket = accept(server_fd, (struct sockaddr *)&client_address, &addrlen);
        if (client_socket < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }
        // 每个连接单独开启线程处理
        std::thread(&Server::handleClient, this, client_socket).detach();
    }
}
