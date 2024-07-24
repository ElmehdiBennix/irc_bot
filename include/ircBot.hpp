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
#include <random> // srand
#include <thread> //  std::this_thread::sleep_for
#include <chrono> //  std::chrono::milliseconds
#include <stdexcept>
// #include <fcntl.h>

#include "defines.hpp"
#include "bot/bot.hpp"
