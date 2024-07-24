#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netdb.h> // gethostbyname
#include <cstring> // memory funcs
#include <unistd.h>
#include <vector>
#include <arpa/inet.h> // For inet_pton and sockaddr_in
#include <map>
#include <sstream> // stingstream
#include <string>
#include <functional> // bind find
#include <random> //  srand
#include <thread> //  std::this_thread::sleep_for
#include <chrono> //  std::chrono::milliseconds
#include <stdexcept>
// #include <fcntl.h>

#include "defines.hpp"
#include "bot/bot.hpp"

//  _TOOLS_:
uint16_t                    valid_port(char *port_arg) throw();
uint16_t                    arg_checker(int ac, char **av) throw();
int                         createTCPSock(void) throw();
bool                        connectSockByName(int socket, const char *hostbyname, int port) throw();
bool                        connectSockByIp(int socket, const char *ip_addr, int port);
inline void                 sendit(int sock, std::string& msg);
std::string                 jsonValue(const std::string& json, const std::string& key) throw();
std::string                 revcit(int sock);
std::string                 trimTheSpaces(const std::string& str) throw();
std::vector<std::string>    splitMessage(const std::string& message) throw();
