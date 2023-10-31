#include <iostream>
#include <winsock2.h>

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
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Error: Socket creation failed!" << std::endl;
        WSACleanup();   // Degister the socket from a Windows Socket
        return 1;
    }

    // Connection to the server
    sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(6969);
    clientAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    const sockaddr* clientAddressName = (struct sockaddr*)&clientAddress;
    int clientErr = connect(clientSocket, clientAddressName, sizeof(clientAddress));
    if (clientErr == SOCKET_ERROR)
    {
        std::cerr << "Error: Fail to Connect!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    else
    {
        std::cout << "Connect Success!" << std::endl;
    }
    

    // Send and receive data
    std::string sentBuffer;
    std::cout << "Enter Your Message: " << std::endl;
    std::getline(std::cin, sentBuffer);

    int byteCount = send(clientSocket, sentBuffer.c_str(), sentBuffer.size(), 0);
    if (byteCount == SOCKET_ERROR)
    {
        std::cerr << "Error: Client Send Error " << WSAGetLastError() << std::endl;
        return 1; 
    }
    else
    {
        std::cout << "Send a total of Byte: " << byteCount << std::endl; 
    }


    // Sending an object
    Data data;
    data.health = 100;
    byteCount = send(clientSocket, (char*)&data, sizeof(data), 0);

    // Disconnect
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
