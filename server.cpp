#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>

class TCPServer {
public:
    TCPServer(int port) {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            perror("Error creating socket");
            exit(1);
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
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

        while(true) {
            handleClientRequest(clientSocket);
        }
        close(clientSocket);
    }

private:
    int serverSocket;
    sockaddr_in serverAddr;
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    void handleClientRequest(int clientSocket) {
        char buffer[1024];
        memset(buffer, 0, 1024);
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived > 0) {
            std::cout << "Received command: " << buffer << std::endl;

            std::string command(buffer);
            if (command.find("GET ") == 0) {
                std::string filename = command.substr(4);
                sendFileContent(clientSocket, filename);
            } else if (command.find("LIST") == 0) {
                sendFileList(clientSocket);
            } else if (command.find("PUT") == 0){
                std::string filename = command.substr(4);
                receiveFile(clientSocket, filename);
            }
            else {
                const char* response = "Unknown command";
                send(clientSocket, response, strlen(response), 0);
            }
        }
    }

    void receiveFile(int clientSocket, const std::string& filename){
        std::ofstream outputFile(filename);

        if (!outputFile.is_open()){
            const char* response = "Error creating file on server";
            send(clientSocket,response, strlen(response), 0);
            return;
        }

        outputFile.close();
        const char* response = "File received successfully";
        send(clientSocket, response, strlen(response), 0);
    }

    void sendFileContent(int clientSocket, const std::string& filename) {
        std::ifstream file(filename);

        if (file) {
            std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            send(clientSocket, fileContent.c_str(), fileContent.size(), 0);
        } else {
            const char* response = "File not found.";
            send(clientSocket, response, strlen(response), 0);
        }
    }

    void sendFileList(int clientSocket){
        std::string fileList;
        const char* path = "/home/nastia/CLionProjects/csc_1homework";

        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir(path)) != NULL){
            while ((ent = readdir(dir)) != NULL){
                fileList += ent ->d_name;
                fileList += "\n";
            }
            closedir(dir);
        } else {
            const char* response = "Error reading directory";
            send(clientSocket, response, strlen(response), 0);
            return;
        }
        send(clientSocket, fileList.c_str(), fileList.size(), 0);
    }
};

int main() {
    int port = 12346;
    TCPServer server(port);

    server.startListening();


    return 0;
}
