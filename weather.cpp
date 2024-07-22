#include "bot.hpp"
#include <arpa/inet.h>

#define HOST "api.openweathermap.org"
#define PORT 80
#define PATH "/data/2.5/weather?q=Khouribga&appid=010b99d131be7191865d4110f87f7db3"
//3


#define API_KEY "010b99d131be7191865d4110f87f7db3"

int main(int ac, char **av) {

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cout << "couldnt open socket" << std::endl;
        return false;
    }

    hostent *name;

    if ((name = gethostbyname(HOST)) == NULL) {
        std::cout << "couldnt resolve the name" << std::endl;
        return false;
    }

    std::cout << "====> h_addr_list :" << std::endl;
    for (int i = 0; name->h_addr_list[i] ; i++)
        std::cout << inet_ntoa(*(struct in_addr *)name->h_addr_list[i]) << std::endl;
    std::cout << "====> h_addrtype :" << std::endl;
    std::cout << name->h_addrtype  << std::endl;
    std::cout << "====> h_aliases :" << std::endl;
    for (int i = 0; name->h_aliases[i] ; i++)
        std::cout << name->h_aliases[i]  << std::endl;
    std::cout << "====> h_lenght :" << std::endl;
    std::cout << name->h_length  << std::endl;
    std::cout << "====> h_name :" << std::endl;
    std::cout << name->h_name  << std::endl;
    std::cout << std::endl;

    sockaddr_in server;
    std::memset((char *)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    std::memcpy((char *)&server.sin_addr.s_addr,(char *)name->h_addr, name->h_length); //
    // server.sin_addr.s_addr = HOST;
    server.sin_port = htons(PORT);


    if (connect(sock, (sockaddr *)&server, sizeof(server)) < 0) {
        std::cout << "cant connect to the server" << std::endl;
        return false;
    }

    std::cout << "connected and following..." << std::endl;
    std::cout << std::endl;

    // Formulate HTTP GET request
    std::string request = "GET " + std::string(PATH) + " HTTP/1.1\r\n";
    request += "Host: " + std::string(HOST) + "\r\n";
    request += "Connection: close\r\n";
    request += "\r\n";
    // request += "Accept: */*\r\n";
    // request += "Accept-Encoding: gzip, deflate, br\r\n";
    // request += "Connection: keep-alive\r\n";


    if (send(sock, request.c_str(), request.length(), 0) < 0) {
        std::cout << "failed to send a request to the host" << std::endl;
        return false;
    }
    
    std::cout << "request was sent to the host" << std::endl;
    std::cout << "====================================" << std::endl;

    // // Read the response
    int bytes = 0;
    char buffer[4096];
    std::string response;

    while ((bytes = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes] = '\0';
        response += buffer;
    }

    if (bytes < 0) {
        std::cerr << "Error reading from socket" << std::endl;
         return false; 
    }

    // // Print the response
    std::cout << "Response:\n" << response << std::endl;

    // // Close the socket
    close(sock);
    return 0;
}


