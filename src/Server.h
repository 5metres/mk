#ifndef SERVER_H
#define SERVER_H

#include "ThreadPool.h"
#include <string>

class Server {
public:
    Server(int port, size_t threadCount);
    ~Server();
    void run();

private:
    int port;
    int server_fd;
    ThreadPool threadPool;
    bool initSocket();
    void handleClient(int client_socket);
};

#endif // SERVER_H
