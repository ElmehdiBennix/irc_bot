#include "../include/ircBot.hpp"

int createTCPSock(void) throw() {
    int newSock;
    
    if ((newSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    // if (fcntl(newSock, F_SETFL, fcntl(newSock, F_GETFL, 0) | O_NONBLOCK) < 0)
    //     return (close(newSock), -1);
    return newSock;
};

bool connectSockByName(int socket, const char *hostbyname, int port) throw() {
    hostent *serverNick;

    if ((serverNick = gethostbyname(hostbyname)) == NULL) {
        close(socket);
        return false;
    }

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    std::memcpy(&server_addr.sin_addr.s_addr, serverNick->h_addr_list[0], serverNick->h_length);
    server_addr.sin_port = htons(port);

    if (connect(socket, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close (socket);
        return false;
    }
    return true;
};

bool connectSockByIp(int socket, const char *ip_addr, int port) {
    sockaddr_in server_addr;

    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_addr);
    server_addr.sin_port = htons(port);

    if (connect(socket, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(socket);
        return false;
    }
    return true;
};

inline void sendit(int sock, std::string& msg) {
    msg += "\r\n";
    // __LOG(msg, BLUE);
    if (send(sock, msg.c_str(), msg.length(), 0) == -1 ) {
        close(sock);
        throw (std::invalid_argument("Error: send failed."));
    }
};

std::string jsonValue(const std::string& json, const std::string& key) throw() {
    size_t keyPos = json.find(key);
    if (keyPos == std::string::npos) return "";

    size_t valueStart = json.find(':', keyPos) + 1;
    if (valueStart == std::string::npos) return "";

    size_t valueEnd = json.find_first_of(",}", valueStart);
    if (valueEnd == std::string::npos) return "";

    std::string value = json.substr(valueStart, valueEnd - valueStart);

    if (value[0] == '"') {
        value = value.substr(1, value.size() - 2);
    }
    return value;
};

std::string revcit(int sock) {
    int bytes = 0;
    char BUFFER[4096];
    
    if ((bytes = recv(sock, BUFFER, sizeof(BUFFER) - 1, MSG_DONTWAIT)) == -1) {
        return "";
    }
    else if (bytes == 0) {
        close(sock);
        throw (std::invalid_argument("connection closed."));
    }
    BUFFER[bytes] = '\0';

    return BUFFER;
};

std::string trimTheSpaces(const std::string& str) throw() {
    size_t first = str.find_first_not_of(" \t\r\n");            
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
};

std::vector<std::string> splitMessage(const std::string& message) throw() {
    std::string part;
    std::vector<std::string> parts;
    std::istringstream iss(message);

    unsigned int skip = 0;

    while (iss >> part) {
        if (skip == 0)
            part = part.substr(part.find_first_not_of(':'), (part.find_first_of('!') - 1));
        else if (skip == 3 && part[0] == ':')
            part.erase(part.begin());
        parts.push_back(part);
        skip++;
    }

    return parts;
};
