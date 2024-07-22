#if !defined(BOT_HPP__)
#define BOT_HPP__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h> // gethostbyname
#include <cstring> // memory funcs
#include <unistd.h>
#include <vector>
#include <arpa/inet.h> // For inet_pton and sockaddr_in
#include <map>
// #include <fcntl.h>

#include <sstream> // stingstream
#include <string>

#include <functional> // bind
#include <random> // srand


// privmsg bot:dsa
// :Irc_42.chat 412 ds :No text to send
// #define HELLO ""

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#define BOT "elgarodumomento"
// #define BOT "bot"
#define LOCALHOST "127.0.0.1"
#define HOST "api.openweathermap.org"
#define PORT 80
// if password is empty string it catches a fatal error // TODO
// ERR_NOTREGISTERED (451) // TODO ADD IT TO IRC AUTHEN 


//fatal errors for bot
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NICKCOLLISION 436
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462
#define ERR_PASSWDMISMATCH 464

//nonfatal errors for bot but will respond to his master


#define __NONFATAL(CODE) (CODE == )


#define __FATAL(CODE) (CODE == ERR_NONICKNAMEGIVEN \
            || CODE == ERR_ERRONEUSNICKNAME \
            || CODE == ERR_NICKNAMEINUSE \
            || CODE == ERR_NICKCOLLISION \
            || CODE == ERR_NEEDMOREPARAMS \
            || CODE == ERR_ALREADYREGISTERED \
            || CODE == ERR_PASSWDMISMATCH)

// #define PATH "/data/2.5/weather?q=Khouribga&appid=010b99d131be7191865d4110f87f7db3"


class Bot {
    private:
        using commandFunc = void (Bot::*) (const std::vector<std::string>&);
        
        std::vector<std::string> masters; // may be added after using moded commands
        int irc_sock;
        int weather_sock;

        bool registerd;

        std::vector<std::string> channels;
        std::string API_KEY; // can be added using the master

        std::map<std::string, commandFunc> commandList;
        std::map<std::string, std::map<std::string, int>> moderationList; // channel -> {name | strikes}

    public:
        Bot(void) {
            std::string master;

            std::cout << "whats ur server Nickname master :";
            if(!getline(std::cin, master) || master.empty()) { // or has spaces
                std::cerr << "Nickname cant be empty nor have eny spaces in it." << std::endl;
                throw (std::exception());
            }

            std::cout << "your Nickname is :" << master << std::endl;
            masters.push_back(master);
            std::cout << "Do you want to continue? [Y/n] ";
            if (!getline(std::cin, master) && master.empty() && (master != "Y" || master != "y")) {
                std::cout << BOT ": Quit." << std::endl;
                throw (std::exception());
            }

            std::cout << "Do u have an API_KEY from api.openweathermap.org? [Y/n] ";
            if (getline(std::cin, master) && !master.empty() && (master == "Y" || master == "y"))
            {
                std::cout << "API_KEY :";
                if(!getline(std::cin, master) || master.empty()) { // or has spaces
                    std::cerr << "API_KEY cant be empty nor have eny spaces in it." << std::endl;
                    throw (std::exception());
                }
            } else {
                std::cout << "cant connect to the openweathermap.org without api." << std::endl; // info
                this->API_KEY = "\0";
            }

            if ((this->irc_sock = createTCPSock()) == -1 || (this->weather_sock = createTCPSock()) == -1) {
                std::cerr << "failed to create socket." << std::endl;
                throw (std::exception());
            }
        };

        int createTCPSock(void) {
            int newSock;
            
            if ((newSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                return -1;
            // if (fcntl(newSock, F_SETFL, fcntl(newSock, F_GETFL, 0) | O_NONBLOCK) < 0)
            //     return (close(newSock), -1);
            return newSock;
        };

        bool connectSockByName(int socket, const char *hostbyname, int port) throw(){
            hostent *serverNick = gethostbyname(hostbyname);
            if (serverNick == NULL) {
                std::cout << "couldnt resolve server ip from name: " << hostbyname << std::endl;
                return false;
            }

            sockaddr_in server_addr;

            std::memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            std::memcpy(&server_addr.sin_addr.s_addr, serverNick->h_addr_list[0], serverNick->h_length);
            server_addr.sin_port = htons(port);

            if (connect(socket, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                std::cout << "couldnt connect to socket address." << std::endl;
                return false;
            }
            return true;
        };


        void connectSockByIp(int socket, const char *ip_addr, int port) {

            sockaddr_in server_addr;

            std::memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = inet_addr(ip_addr);
            server_addr.sin_port = htons(port);

            if (connect(socket, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                std::cout << "couldnt connect to socket address." << std::endl; // close sock
                throw (std::exception());
            }
        };


        void authenticate(std::string password) { // handle failed auth
            std::string message;

            usleep(200);
            message = "PASS " + password;
            sendit(this->irc_sock, message);


            usleep(200);
            message = "NICK " BOT;
            sendit(this->irc_sock, message);

            usleep(200);
            message = "USER " BOT " 0 * : " BOT;
            sendit(this->irc_sock, message);
        };


        inline void sendit(int sock, std::string& msg) {
            msg += "\r\n";
            if (send(sock, msg.c_str(), msg.length(), 0) == -1 ) {
                throw std::exception();
            }
        };

        std::string revcit(int sock) {
            int bytes = 0;
            char BUFFER[4096];
            
            if ((bytes = recv(sock, BUFFER, sizeof(BUFFER) - 1, MSG_DONTWAIT)) == -1) {
                return "";
            }
            else if (bytes == 0) {
                std::cout << "connection closeed." << std::endl;
                throw (std::exception());
            }
            BUFFER[bytes] = '\0';

            return BUFFER;
        };

        void recvError(const std::string &message) {
            std::istringstream stream(message);
            std::string output;
            int code;

            stream >> output;
            stream >> code;

            if __FATAL(code)
                throw(std::exception());
        }

        std::string trimTheSpaces(const std::string& str)
        {
            // Find the first character that is not a space or tab or other whitespace
            size_t first = str.find_first_not_of(" \t\r\n");
            
            // If all characters are whitespace, return an empty string
            if (first == std::string::npos)
                return "";

            // Find the last character that is not a space or tab or other whitespace
            size_t last = str.find_last_not_of(" \t\r\n");

            // Extract the substring between first and last non-whitespace characters
            return str.substr(first, last - first + 1);
        };

        std::vector<std::string> splitMessage(const std::string& message) {
            std::string part;
            std::vector<std::string> parts;
            std::istringstream iss(message);

            unsigned int skip = 0;

            while (iss >> part) {
                std::cout << YELLOW "part " << skip << " |" << part << "|" RESET << std::endl;
                if (skip == 0)
                    part = part.substr(part.find_first_not_of(':'), (part.find_first_of('!') - 1));
                else if (skip == 3 && part[0] == ':')
                    part.erase(part.begin());
                parts.push_back(part);
                skip++;
            }

            return parts;
        }

        void listenForCommand() {
            while (true) {
                std::string message = revcit(this->irc_sock);
                recvError(message);

                // std::cout << GREEN << message << RESET;
        		message = trimTheSpaces(message);

        		std::vector<std::string> fields = splitMessage(message);

                if (!fields.empty()) {

                    int i = 0;
                    std::cout << RED;
                    // std::cout <<"size = " << fields.size() << std::endl;
                    for (const std::string& field : fields) {
                        std::cout << "field " <<  i++ << " |" << field << "|"<< std::endl;
                    }
                    std::cout << RESET;

                //=========== if commads are field 0 from server
                    if (fields[0] == "PING") {
                        (this->*commandList[fields[0]])(fields);
                    } else if (fields.size() > 1 && commandList.find(fields[1]) != commandList.end()) {
                        (this->*commandList[fields[1]])(fields);
                    } else if (fields.size() > 2 && fields[1] == "PRIVMSG" && commandList.find(fields[3]) != commandList.end()) {
                        (this->*commandList[fields[3]])(fields);
                    }

                //===========
                }
            }
        };


        void    tossCommand(const std::vector<std::string> &fields) { // can reply to the channel
            int result;
            std::string command;

            srand(static_cast<unsigned int>(time(0)));
            result = rand() % 2;

            command = "PRIVMSG " + (fields[2] == BOT) ? fields[0] : fields[2] + " :flipping a coin ...";
            sendit(this->irc_sock, command);
            
            command = "PRIVMSG " + (fields[2] == BOT) ? fields[0] : fields[2] + ((result == 0) ? " :Heads !!!" : " :Tails !!!");
            sendit(this->irc_sock, command);
        };
  
        void    weatherCommand(const std::vector<std::string> &fields) {  // can reply to the channel

        };

        void    pollCommand(const std::vector<std::string> &fields) {  // can reply to the channel
            // Creates and manages polls within the channel.
            // Collects votes and displays results.
        };

        void    helpCommand(const std::vector<std::string> &fields) { // can reply to the channel

        };

        void    monitoringCommand(const std::vector<std::string> &fields) {
            // show data to admins about users and time left for ban
        };




        void    welcomeMsg(const std::vector<std::string> &fields) {
            std::string command = "PRIVMSG " + fields[0] + " :This " + fields[2] + " channel is moderated by " BOT " be polite !!!";
            sendit(this->irc_sock, command);
        };

        void    monitoringSystem(const std::vector<std::string> &fields) {

        };

        void    pongCommand(const std::vector<std::string> &fields) {
            std::string command = "PONG " + fields[1];
            sendit(this->irc_sock, command);
        };


        bool    masterStatus(const std::string &name) {
            std::string command = "PRIVMSG ";

            for (const std::string &master : masters) {
                if (name == master)
                    return true;
            }
            command += name + " : u are not one of my masters, I cant help you.";
            sendit(this->irc_sock, command);
            return false;
        }

        void    adminCommand(const std::vector<std::string> &fields) {
            std::string command = "PRIVMSG ";

            if (!masterStatus(fields[0]))
                return;

            if (fields.size() >= 5)
            {
                if (fields[4] == "join") {

                } else if (fields[4] == "part") {
                
                } else if (fields[4] == "add") {
                
                } else if (fields[4] == "del") {

                } else 
                    helpCommand(fields);
            }

            // /admin join channel
            // /admin part channel
            // /admin add master
            // /admin del master

            // monitoring command
        };


        void startBot(uint16_t port, const char *password) {
            std::string answer;

// connect to the irc server:
            std::cout << "Connect to localhost [Y/n] ";
            if (getline(std::cin, answer) && !answer.empty() && (answer == "Y" || answer == "y"))
            {
                connectSockByIp(this->irc_sock, LOCALHOST, port); // error
            } else { // by name or ip
                std::cout << "Irc Server ip: "; // hardcoded for now 
                getline(std::cin, answer);
                connectSockByName(this->irc_sock, "irc.libera.chat", 6667);
                // connectSockByIp(this->irc_sock, answer.c_str(), port);
            }

            authenticate(password);

// connect to eny external api:
            // if (connectSockByName(this->weather_sock, HOST, PORT)) {
                // commandList["/weather"] = &Bot::pongCommand;
            // }
            commandList["PING"] = &Bot::pongCommand;
            commandList["JOIN"] = &Bot::welcomeMsg;
            commandList["/toss"] = &Bot::tossCommand;
            commandList["/admin"] = &Bot::adminCommand;
            commandList["/help"] = &Bot::helpCommand;

            listenForCommand();
        }


        ~Bot() {};
};

#endif