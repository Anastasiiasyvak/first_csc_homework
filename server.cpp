#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

class TCPServer {
public:
    TCPServer(int port) {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            perror("Error creating socket");
            exit(1);
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY; // сервер приймає з'єднання на будь-якому доступному IP-адресі цього пристрою
        serverAddr.sin_port = htons(port);

        if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
            perror("Bind failed");
            close(serverSocket);
            exit(1);
        }

        if (listen(serverSocket, SOMAXCONN) == -1) {
            perror("Listen failed");
            close(serverSocket);
            exit(1);
        }

        std::cout << "Server listening on port " << port << std::endl;
    }

    ~TCPServer() {
        close(serverSocket);
    }

    void startListening() {
        sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1) {
            perror("Accept failed");
            close(serverSocket);
            exit(1);
        }

        std::cout << "Accepted connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) <<
                  std::endl;

        char buffer[1024];
        memset(buffer, 0, 1024);
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            std::cout << "Received data: " << buffer << std::endl;

            const char* response = "Hello, client! This is the server.";
            send(clientSocket, response, strlen(response), 0);
        }

        close(clientSocket);
    }

private:
    int serverSocket;
    sockaddr_in serverAddr;
};



int main() {
    int port = 12345;
    TCPServer server(port);

    server.startListening();

    return 0;
}
