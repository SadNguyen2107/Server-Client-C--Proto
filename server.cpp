#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

struct Data
{
    int health;
};

int main(int argc, char const *argv[])
{
    // Init the Window Socket API
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2,2);
    int wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0)
    {
        std::cerr << "Error: WSAStartup failed!" << std::endl;
        return 1; 
    }

    // Create A Socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Error: Socket creation failed!" << std::endl;
        WSACleanup();   // Degister the socket from a Windows Socket
        return 1;
    }
    
    // Bind the socket
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(6969);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    const sockaddr* serverAddressName = (struct sockaddr*)&serverAddress;
    int serverErr = bind(serverSocket, serverAddressName, sizeof(serverAddress));
    if (serverErr == SOCKET_ERROR)
    {
        std::cerr << "Error: Bind Failed!" << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen on the socket
    //? Backlog: number of connection allowed
    int listenErr = listen(serverSocket, 10);
    if (listenErr == SOCKET_ERROR)
    {
        std::cerr << "Error: Lisen Error" << WSAGetLastError() << std::endl;
    }
    else
    {
        std::cout << "Server Is Listenning..." << std::endl;
    }

    // Accept a connection
    // Set up another socket
    SOCKET clientSocket;
    clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Error: Accept Failed!" << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    

    // Send and receive data
    char receiveBuffer[1024];
    int byteCount = recv(clientSocket, receiveBuffer, sizeof(receiveBuffer), 0);
    if (byteCount < 0)
    {
        std::cerr << "Error: Client send Error!" << WSAGetLastError() << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Receiving Data: " << receiveBuffer << std::endl;
    }
    
    // Receiving an Object
    Data data;
    byteCount = recv(clientSocket, (char*)&data, sizeof(data), 0);
    std::cout << "Health: " << data.health << std::endl;

    // Disconnect
    closesocket(serverSocket);


    

    return 0;
}
