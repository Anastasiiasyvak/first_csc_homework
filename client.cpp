#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

class TCPClient {
public:
    TCPClient(const char* serverIp, int port) {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            perror("Error creating socket");
            exit(1);
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, serverIp, &(serverAddr.sin_addr));
    }

    ~TCPClient() {
        close(clientSocket);
    }

    bool connectToServer() {
        return connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) != -1;
    }

    void sendMessage(const char* message) {
        send(clientSocket, message, strlen(message), 0);
    }

    void receiveMessage() {
        char buffer[1024];
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            std::cout << "Received from server: " << buffer << std::endl;
        }
    }

private:
    int clientSocket;
    sockaddr_in serverAddr;
};

int main() {
    const char* serverIp = "127.0.0.1";
    int port = 12345;

    TCPClient client(serverIp, port);

    if (!client.connectToServer()) {
        perror("Connect failed");
        return 1;
    }

    const char* message = "Hello, server! How are you?";
    client.sendMessage(message);

    client.receiveMessage();

    return 0;
}
