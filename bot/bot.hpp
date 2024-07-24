#if !defined(BOT_HPP__)
#define BOT_HPP__

#include "../include/ircBot.hpp"

class Bot {
    private:
        int irc_sock;

        std::string API_KEY;

        std::vector<std::string> masters;
        std::vector<std::string> channels;

        using commandFunc = void (Bot::*) (const std::vector<std::string>&);
        std::map<std::string, commandFunc> commandList;

    public:
        Bot(void) {
            std::string master;

            std::cout << "whats ur server Nickname master :";
            getline(std::cin, master);
            if(master.empty()) // or has spaces
                throw (std::invalid_argument("Nickname cant be empty nor have eny spaces in it."));

            std::cout << YELLOW "your Nickname is :" << master << RESET << std::endl;
            masters.push_back(master);
            std::cout << "Do you want to continue? [Y/n] ";
            getline(std::cin, master);
            if (master.empty() || (master != "Y" && master != "y"))
                throw (std::invalid_argument(BOT ": Quit." ));

            std::cout << "Do u have an API_KEY from api.openweathermap.org? [Y/n] ";
            if (getline(std::cin, master) && !master.empty() && (master == "Y" || master == "y"))   // optimize this
            {
                std::cout << "API_KEY :";
                getline(std::cin, master);
                if(master.empty()) { // or has spaces
                    std::cout << "API_KEY cant be empty nor have eny spaces in it." << std::endl;
                    std::cout << "API will be set to null u can configure it later." << std::endl; // info
                    this->API_KEY = "\0";
                } else 
                    this->API_KEY = master;
            } else {
                std::cout << "API will be set to null u can configure it later." << std::endl; // info
                this->API_KEY = "\0";
            }

            if ((this->irc_sock = createTCPSock()) == -1)
                throw (std::invalid_argument("failed to create socket."));

            commandList["PING"] = &Bot::pongCommand;
            commandList["JOIN"] = &Bot::welcomeMsg;
            commandList["PART"] = &Bot::leaveMsg;
            commandList["/help"] = &Bot::helpCommand;
            commandList["/joke"] = &Bot::jokeCommand;
            commandList["/toss"] = &Bot::tossCommand;
            commandList["/weather"] = &Bot::weatherCommand;
            // commandList["/poll"] = &Bot::;
            commandList["/admin"] = &Bot::adminCommand;
        };

        int createTCPSock(void) const throw() {
            int newSock;
            
            if ((newSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                return -1;
            // if (fcntl(newSock, F_SETFL, fcntl(newSock, F_GETFL, 0) | O_NONBLOCK) < 0)
            //     return (close(newSock), -1);
            return newSock;
        };

        bool connectSockByName(int socket, const char *hostbyname, int port) const throw() {
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


        bool connectSockByIp(int socket, const char *ip_addr, int port) const {
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

        void authenticate(std::string password) const {
            std::string message;

            if (password.empty())
                password = "NAN";

            message = "PASS " + password + "\r\nNICK " BOT "\r\nUSER " BOT " 0 * : " BOT;

            sendit(this->irc_sock, message);

            // usleep(200);
            // sendit(this->irc_sock, message);

            // usleep(200);
            // sendit(this->irc_sock, message);
        };


        inline void sendit(int sock, std::string& msg) const {
            msg += "\r\n";
            // __LOG(msg, BLUE);
            if (send(sock, msg.c_str(), msg.length(), 0) == -1 ) {
                close(sock);
                throw (std::invalid_argument("Error: send failed."));
            }
        };

        std::string jsonValue(const std::string& json, const std::string& key) const throw() {
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
        }

        std::string revcit(int sock) const {
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

        void    logger(const std::string &message) const {
            int code;
            std::string output;
            std::istringstream stream(message);

            stream >> output;
            stream >> code;

            if __FATAL(code) {
                close(this->irc_sock);
                throw (std::invalid_argument(message));
            } else if __NONFATAL(code)
                __LOG(message, YELLOW)
            else
                __LOG(message, GREEN)
        }

        std::string trimTheSpaces(const std::string& str) const throw() {
            size_t first = str.find_first_not_of(" \t\r\n");            
            if (first == std::string::npos)
                return "";
            size_t last = str.find_last_not_of(" \t\r\n");
            return str.substr(first, last - first + 1);
        };

        std::vector<std::string> splitMessage(const std::string& message) const throw() {
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
        }

        void listenForCommand(void) {
            while (true) {
                std::string message;
                if ((message = revcit(this->irc_sock)) == "")
                    continue;

        		message = trimTheSpaces(message);
        		std::vector<std::string> fields = splitMessage(message);

                if (!fields.empty()) {
                    logger(message);

                    if (fields[0] == "PING") {
                        (this->*commandList[fields[0]])(fields);
                    } else if (fields.size() > 1 && commandList.find(fields[1]) != commandList.end()) {
                        (this->*commandList[fields[1]])(fields);
                    } else if (fields.size() > 2 && fields[1] == "PRIVMSG" && commandList.find(fields[3]) != commandList.end()) {
                        (this->*commandList[fields[3]])(fields);
                    }
                }
            }
        };

        void    jokeCommand(const std::vector<std::string> &fields) {
            const char* jokes[] = JOKES;

            const int numJokes = sizeof(jokes) / sizeof(jokes[0]);
            const std::string &client = __GETARGET;
            std::string command = "PRIVMSG " + client + " :";

            srand(static_cast<unsigned>(time(0)));

            int randomIndex = rand() % numJokes;

            command += jokes[randomIndex];
            sendit(this->irc_sock, command);
        };

        void    tossCommand(const std::vector<std::string> &fields) {
            int result;
            std::string command;
            const std::string &client = __GETARGET;

            srand(static_cast<unsigned int>(time(0)));
            result = rand() % 2;

            command = "PRIVMSG " + client + " :flipping a coin ...";
            sendit(this->irc_sock, command);

            command = "PRIVMSG " + client + ((result == 0) ? " :Heads !!!" : " :Tails !!!");
            sendit(this->irc_sock, command);
        };

        const std::map<std::string, std::string> extractWeatherData(const std::string &response) {
            std::map<std::string, std::string> weatherData;

            weatherData["longitude"] = jsonValue(response, "lon");
            weatherData["latitude"] = jsonValue(response, "lat");
            weatherData["main"] = jsonValue(response, "main");
            weatherData["description"] = jsonValue(response, "description");
            weatherData["temperature"] = jsonValue(response, "temp");
            weatherData["feels_like"] = jsonValue(response, "feels_like");
            weatherData["pressure"] = jsonValue(response, "pressure");
            weatherData["humidity"] = jsonValue(response, "humidity");
            weatherData["visibility"] = jsonValue(response, "visibility");
            weatherData["wind_speed"] = jsonValue(response, "speed");
            weatherData["wind_deg"] = jsonValue(response, "deg");
            weatherData["clouds"] = jsonValue(response, "all");
            weatherData["country"] = jsonValue(response, "country");
            weatherData["city"] = jsonValue(response, "name");

            return weatherData;
        }

        std::string getWeatherData(const std::map<std::string, std::string>& weatherData , const std::string &client) {
            std::ostringstream message;

            try {
                message << "PRIVMSG " << client << " :Weather report for " << weatherData.at("city") << ", " << weatherData.at("country") << ":\r\n";
                message << "PRIVMSG " << client << " :Coordinates: (" << weatherData.at("latitude") << ", " << weatherData.at("longitude") << ")\r\n";
                message << "PRIVMSG " << client << " :Weather: " << weatherData.at("main") << " (" << weatherData.at("description") << ")\r\n";
                message << "PRIVMSG " << client << " :Temperature now: " << std::stof(weatherData.at("temperature")) - 273.15 << "°C (feels like " << std::stof(weatherData.at("feels_like")) - 273.15 << "°C)\r\n";
                message << "PRIVMSG " << client << " :Pressure: " << weatherData.at("pressure") << " hPa\r\n";
                message << "PRIVMSG " << client << " :Humidity: " << weatherData.at("humidity") << "%\r\n";
                message << "PRIVMSG " << client << " :Visibility: " << weatherData.at("visibility") << " meters\r\n";
                message << "PRIVMSG " << client << " :Wind: " << weatherData.at("wind_speed") << " m/s at " << weatherData.at("wind_deg") << "°\r\n";
                message << "PRIVMSG " << client << " :Cloudiness: " << weatherData.at("clouds") << "%\r\n";
            }
            catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
            return (message.str());
        }


        void    weatherCommand(const std::vector<std::string> &fields) {  // can reply to the channel
            std::string command;
            const std::string &client = __GETARGET;

            if (fields.size() > 4)
            {
                int weather_sock;
                int bytes = 0;
                char buffer[4096];

                if ((weather_sock = createTCPSock()) == -1 || !connectSockByName(weather_sock, HOST, PORT))
                {
                    command = "PRIVMSG " + client + " :failed to connect to " HOST " server";
                    return;
                }

                std::map<std::string, std::string> weatherData;
                std::string response;
                std::string request = "GET /data/2.5/weather?q=" + fields[4] + "&appid=" + API_KEY + " HTTP/1.1\r\n";
                request += "Host: " HOST "\r\n";
                request += "Connection: close\r\n";

                sendit(weather_sock, request);

                while ((bytes = recv(weather_sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
                    buffer[bytes] = '\0';
                    response += buffer;
                }

                if (bytes < 0)
                    command = "PRIVMSG " + client + " :failed to get weather Data from " HOST " server";                
                else if (jsonValue(response, "cod") == "200") {
                    std::map<std::string, std::string> weatherData = extractWeatherData(response);
                    command = getWeatherData(weatherData, client);
                }
                else
                    command = "PRIVMSG " + client + " :/weather " + jsonValue(response, "message");
                close(weather_sock);
            } else {
                command = "PRIVMSG " + client + " :/weather missing argument.";
            }
            sendit(this->irc_sock, command);
        };

        // void    pollCommand(const std::vector<std::string> &fields) {  // can reply to the channel
        //     std::string command;
        //     const std::string &client = __GETARGET;
        //     // Creates and manages polls within the channel.
        //     // Collects votes and displays results.
        // };

        void    helpCommand(const std::vector<std::string> &fields) { // can reply to the channel
            std::string command;
            const char *manual[] = MANUAL;
            const std::string &client = __GETARGET;  // make a loop of 200ms delay between msgs
 
            int total = sizeof(manual) / sizeof(manual[0]);

            for (int i = 0; i < total; i++) {
                command += "PRIVMSG " + client + manual[i] + "\r\n";
            }
            sendit(this->irc_sock, command);
        };

        inline void    welcomeMsg(const std::vector<std::string> &fields) {
            std::string command = "PRIVMSG " + fields[0] + WELCOME(fields[2]); 
            sendit(this->irc_sock, command);
        };

        inline void    leaveMsg(const std::vector<std::string> &fields) {
            std::string command = "PRIVMSG " + fields[0] + FAREWELL(fields[2]);
            sendit(this->irc_sock, command);
        };

        inline void    pongCommand(const std::vector<std::string> &fields) {
            std::string command = "PONG " + fields[1];
            sendit(this->irc_sock, command);
        };

        bool    masterStatus(const std::string &name) const {
            std::string command;

            for (const std::string &master : masters) {
                if (name == master)
                    return true;
            }
            command = "PRIVMSG " + name + SUDOERROR;
            sendit(this->irc_sock, command);
            return false;
        }

        void    adminCommand(const std::vector<std::string> &fields) {
            std::string command;
            const std::string &action = fields[4];
            std::vector<std::string>::iterator it;

            if (!masterStatus(fields[0]))
                return;

            if (fields.size() > 5)
            {
                if (action == "join") {
                    if ((it = std::find(channels.begin(), channels.end(), fields[5])) == channels.end()) {
                        command = "JOIN " + fields[5];
                        channels.push_back(fields[5]);
                        sendit(this->irc_sock, command);
                    }
                } else if (action == "part") {
                    if ((it = std::find(channels.begin(), channels.end(), fields[5])) != channels.end()) {
                        command = "PART " + fields[5];
                        channels.erase(it);
                        sendit(this->irc_sock, command);
                    }
                } else if (action == "add") {
                    if ((it = std::find(channels.begin(), channels.end(), fields[5])) == channels.end()) {
                        masters.push_back(fields[5]);
                    }
                } else if (action == "del") {
                    if ((it = std::find(masters.begin(), masters.end(), fields[5])) != masters.end()) {
                        masters.erase(it);
                    }
                } else if (action == "set") {
                        API_KEY = fields[5];
                } else if (action == "list") {
                    command = "PRIVMSG " + fields[0];
                    if (fields[5] == "masters") {
                        command += " :Masters =";
                        for (const std::string name: masters) {
                            command += " " + name;
                        }
                    } else if (fields[5] == "channels") {
                        command += " :Channels =";
                        for (const std::string name: channels) {
                            command += " " + name;
                        }
                    } else
                        command += SYNXERROR;
                    sendit(this->irc_sock, command);
                } else {
                    command = "PRIVMSG " + fields[0] + SYNXERROR;
                    sendit(this->irc_sock, command);
                }
            } else {
                command = "PRIVMSG " + fields[0] + SYNXERROR;
                sendit(this->irc_sock, command);
            }
        };


        void startBot(uint16_t port) {
            std::string answer;

            std::cout << "connect using :\n     1 - using LOCALHOST.\n     2 - using ip address.\n     3 - using domain name.\n" << std::endl; // hardcoded for now 
            std::cout << "Answer [1/2/3] ";

            getline(std::cin, answer);
            if (answer == "1") {
                if (!connectSockByIp(this->irc_sock, LOCALHOST, port))
                    throw (std::invalid_argument("couldnt connect to socket address."));
            } else if (answer == "2") {
                std::cout << "Irc Server ip: ";
                getline(std::cin, answer);
                if (!connectSockByIp(this->irc_sock, answer.c_str(), port))
                    throw (std::invalid_argument("couldnt connect to socket address."));
            } else if (answer == "3") {
                std::cout << "Irc server name: ";
                getline(std::cin, answer);
                if (!connectSockByName(this->irc_sock, answer.c_str(), port))
                    throw (std::invalid_argument("couldnt connect to socket address."));
            } else {
                close(this->irc_sock);
                throw (std::invalid_argument("Only [1/2/3] are allowed."));
            }
        }

        // inline void    unavailbleCommand(const std::vector<std::string> &fields) {
        //     const std::string &client = __GETARGET;
        //     std::string commnad = "PRIVMSG " + client + " :" + fields[3] + "command unavailbe, contact a master";
        // }

        ~Bot() {};
};

#endif