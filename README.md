# **Communication Protocol Description**

**Short description**

This project consists of a simple TCP client-server file transfer application. The server allows clients to perform basic file-related operations such as retrieving file content, listing files in a directory, uploading files to the server, deleting files, and retrieving file information

**Commands**

- GET <filename>: Retrieve the content of the specified file from the server.
  (Example: GET file.txt)

- LIST: List all files in the server's directory.
  (Example: LIST)

- PUT <filename>: Upload a file to the server.
  (Example: PUT first.txt)

- DELETE <filename>: Delete the specified file on the server.
  (Example: DELETE file.txt)

- INFO <filename>: Retrieve information about the specified file on the server.
  (Example: INFO file.txt)

**Compilation and Execution Requirements**

- To compile the server: `g++ server.cpp -o server`
- To run the server: `./server`

- To compile the client: `g++ client.cpp -o client`
- To run the client: `./client`

**Interaction Process**

1. The server starts on port 12346
2. The client initiates a connection to the server.
3. The server listens for incoming connections.
4. Once connected, the client sends commands to the server.
5. The server processes the commands and responds accordingly.
6. Data transfer (if applicable) occurs between the client and server.
7. The connection is closed when the interaction is complete.
