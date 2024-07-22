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

// log activity to a file 


//fatal errors for bot
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NICKCOLLISION 436
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462
#define ERR_PASSWDMISMATCH 464

//nonfatal errors for bot but will respond to his master

#define __FATAL(CODE) (CODE == ERR_NONICKNAMEGIVEN \
            || CODE == ERR_ERRONEUSNICKNAME \
            || CODE == ERR_NICKNAMEINUSE \
            || CODE == ERR_NICKCOLLISION \
            || CODE == ERR_NEEDMOREPARAMS \
            || CODE == ERR_ALREADYREGISTERED \
            || CODE == ERR_PASSWDMISMATCH)

#define PATH "/data/2.5/weather?q=Khouribga&appid=010b99d131be7191865d4110f87f7db3"


// Available Commands to invoke:
// --------------------------
// /help                 - Displays this help message.
// /joke                 - Tells a random joke.
// /toss                 - Tosses a coin: head or tails.
// /weather {city}       - Gets weather info from the API for the specified city.
// /poll {option} {arg}  - Poll command with options:
//      - list                    : List all available polls.
//      - create {question}       : Create a new poll with the given question.
//      - vote {poll_id}          : Vote for a poll option.
// /admin {option} {arg} - Admin command with options:
//      - add {user}        : Add a user to the master list.
//      - del {user}        : Remove a user from the master list.
//      - join {channel}    : Join the specified channel.
//      - part {channel}    : Leave the specified channel.
// - set
//      - display {option}  : Display specific information.
//               - masters          : List all users with master privileges.
//               - channels         : List all channels the bot is currently in.

#define SYNXERROR "There was an error while parsing the command please use /help."
#define SUDOERROR "U re not a memeber of the masters list,"

#define JOKES { \
                "Why don't scientists trust atoms? Because they make up everything!", \
                "Why did the scarecrow win an award? Because he was outstanding in his field.", \
                "Why did the bicycle fall over? It was two-tired.", \
                "How does a penguin build its house? Igloos it together.", \
                "What do you get when you cross a snowman and a vampire? Frostbite.", \
                "Why don't skeletons fight each other? They don't have the guts.", \
                "What do you call cheese that isn't yours? Nacho cheese.", \
                "Why couldn't the leopard play hide and seek? Because he was always spotted.", \
                "What do you get when you cross a joke with a rhetorical question?", \
                "How does a lion greet the other animals in the field? 'Pleased to eat you!'", \
                "Why don't programmers like nature? It has too many bugs.", \
                "What do you call fake spaghetti? An impasta.", \
                "How does a robot eat guacamole? With a micro-chip.", \
                "Why do cows have hooves instead of feet? Because they lactose.", \
                "What did the janitor say when he jumped out of the closet? 'Supplies!'", \
                "Why did the golfer bring two pairs of pants? In case he got a hole in one.", \
                "What does a cloud wear under his raincoat? Thunderwear.", \
                "Why did the tomato turn red? Because it saw the salad dressing!", \
                "What do you call a factory that makes good products? A satisfactory.", \
                "Why don't some couples go to the gym? Because some relationships don't work out.", \
                "What do you call a bear with no teeth? A gummy bear.", \
                "What does one wall say to the other? 'I'll meet you at the corner.'", \
                "Why was the computer cold? It left its Windows open.", \
                "What did the left eye say to the right eye? 'Between you and me, something smells.'", \
                "Why was the belt arrested? For holding up a pair of pants.", \
                "What did the ocean say to the beach? Nothing, it just waved.", \
                "How do you catch a squirrel? Climb a tree and act like a nut!", \
                "What did one hat say to the other? 'You stay here, I'll go on ahead!'", \
                "Why did the golfer bring extra socks? In case he got a hole in one.", \
                "What did the big flower say to the little flower? 'Hey, bud!'", \
                "How do you organize a space party? You planet.", \
                "What do you call a fish with no eyes? Fsh.", \
                "Why did the cookie go to the hospital? Because it felt crummy.", \
                "What do you call an alligator in a vest? An investigator.", \
                "Why did the scarecrow become a successful neurosurgeon? Because he was outstanding in his field.", \
                "What do you call a bear that’s stuck in the rain? A drizzly bear.", \
                "What do you get when you cross a snowman and a dog? Frostbite.", \
                "How do you make a tissue dance? Put a little boogie in it.", \
                "Why did the orange stop? It ran out of juice.", \
                "How does a farmer count his cows? With a cow-culator.", \
                "What did one plate say to the other plate? 'Lunch is on me.'", \
                "Why did the bicycle fall over? It was two-tired.", \
                "What do you get when you cross a dinosaur with a pig? Jurassic Pork.", \
                "What do you call a pile of cats? A meow-tain.", \
                "How do you make holy water? You boil the hell out of it.", \
                "Why did the picture go to jail? Because it was framed.", \
                "What did the grape do when it got stepped on? Nothing but let out a little wine.", \
                "What did the janitor say when he jumped out of the closet? 'Supplies!'", \
                "Why did the scarecrow win an award? Because he was outstanding in his field.", \
                "What kind of music do mummies listen to? Wrap music.", \
                "How do you catch a squirrel? Climb a tree and act like a nut.", \
                "What did one wall say to the other wall? 'I'll meet you at the corner.'", \
                "How does a penguin build its house? Igloos it together.", \
                "Why did the golfer bring two pairs of pants? In case he got a hole in one.", \
                "Why don't you ever see elephants hiding in trees? Because they're so good at it.", \
                "Why don't skeletons fight each other? They don't have the guts.", \
                "What do you call a can opener that doesn’t work? A can’t opener.", \
                "Why did the tomato turn red? Because it saw the salad dressing!", \
                "What did the ocean say to the shore? Nothing, it just waved.", \
                "Why do cows wear bells? Because their horns don't work.", \
                "How does a lion greet the other animals in the field? 'Pleased to eat you!'", \
                "Why did the math book look sad? Because it had too many problems.", \
                "Why did the cookie go to the hospital? Because it felt crummy.", \
                "What’s a skeleton’s least favorite room in the house? The living room.", \
                "How do you make a tissue dance? You put a little boogie in it.", \
                "What do you call a fish wearing a bowtie? Sofishticated.", \
                "Why did the bicycle fall over? It was two-tired.", \
                "How does a farmer count his cows? With a cow-culator.", \
                "Why don’t some couples go to the gym? Because some relationships don’t work out.", \
                "How does a penguin build its house? Igloos it together.", \
                "What did one hat say to the other? 'You stay here, I'll go on ahead!'", \
                "Why did the golfer bring two pairs of pants? In case he got a hole in one.", \
                "How do you organize a space party? You planet.", \
                "What did one wall say to the other? 'I'll meet you at the corner.'", \
                "What did the grape do when it got stepped on? Nothing but let out a little wine.", \
                "What do you call cheese that isn’t yours? Nacho cheese.", \
                "Why did the math book look sad? It had too many problems.", \
                "How do you make holy water? You boil the hell out of it.", \
                "What do you call a bear with no teeth? A gummy bear.", \
                "What did the big flower say to the little flower? 'Hey, bud!'", \
                "How do you make a tissue dance? Put a little boogie in it." \
            }

class Bot {
    private:
        using commandFunc = void (Bot::*) (const std::vector<std::string>&);
        
        int irc_sock;
        int weather_sock;

        std::string API_KEY;

        std::vector<std::string> masters;
        std::vector<std::string> channels;

        std::map<std::string, commandFunc> commandList;

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
                    std::cout << "API_KEY cant be empty nor have eny spaces in it." << std::endl;
                    std::cout << "API will be set to null u can configure it later." << std::endl; // info
                    this->API_KEY = "\0";
                }
            } else {
                std::cout << "API will be set to null u can configure it later." << std::endl; // info
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

            if (password.empty())
                password = "NAN";
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

        std::string extractValue(const std::string& json, const std::string& key) {
            std::size_t keyPos = json.find("\"" + key + "\"");
            if (keyPos == std::string::npos) {
                return "";
            }

            std::size_t colonPos = json.find(":", keyPos);
            if (colonPos == std::string::npos) {
                return "";
            }

            std::size_t valueStart = json.find_first_not_of(" \":,", colonPos);
            if (valueStart == std::string::npos) {
                return "";
            }

            std::size_t valueEnd;
            if (json[valueStart] == '"') {
                valueStart++;
                valueEnd = json.find('"', valueStart);
            } else {
                valueEnd = json.find_first_of(",}", valueStart);
            }

            if (valueEnd == std::string::npos) {
                return "";
            }

            return json.substr(valueStart, valueEnd - valueStart);
        }

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

                    // int i = 0;
                    // std::cout << RED;
                    // // std::cout <<"size = " << fields.size() << std::endl;
                    // for (const std::string& field : fields) {
                    //     std::cout << "field " <<  i++ << " |" << field << "|"<< std::endl;
                    // }
                    // std::cout << RESET;

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

        void    jokeCommand(const std::vector<std::string> &fields) { // can reply to the channel
            const char* jokes[] = JOKES;

            const int numJokes = sizeof(jokes) / sizeof(jokes[0]);
            std::string client = (fields[2] == BOT) ? fields[0] : fields[2];
            std::string command = "PRIVMSG " + client + " :";

            srand(static_cast<unsigned>(time(0)));

            int randomIndex = rand() % numJokes;

            command += jokes[randomIndex];
            sendit(this->irc_sock, command);
        };

        void    tossCommand(const std::vector<std::string> &fields) { // can reply to the channel
            int result;
            std::string command;
            std::string client = (fields[2] == BOT) ? fields[0] : fields[2];

            srand(static_cast<unsigned int>(time(0)));
            result = rand() % 2;

            command = "PRIVMSG " + client + " :flipping a coin ...";
            sendit(this->irc_sock, command);
            
            command = "PRIVMSG " + client + ((result == 0) ? " :Heads !!!" : " :Tails !!!");
            sendit(this->irc_sock, command);
        };
  
        // void printIp() {
        //     std::cout << "====> h_addr_list :" << std::endl;
        //     for (int i = 0; name->h_addr_list[i] ; i++)
        //         std::cout << inet_ntoa(*(struct in_addr *)name->h_addr_list[i]) << std::endl;
        //     std::cout << "====> h_addrtype :" << std::endl;
        //     std::cout << name->h_addrtype  << std::endl;
        //     std::cout << "====> h_aliases :" << std::endl;
        //     for (int i = 0; name->h_aliases[i] ; i++)
        //         std::cout << name->h_aliases[i]  << std::endl;
        //     std::cout << "====> h_lenght :" << std::endl;
        //     std::cout << name->h_length  << std::endl;
        //     std::cout << "====> h_name :" << std::endl;
        //     std::cout << name->h_name  << std::endl;
        //     std::cout << std::endl;
        // }

        void    weatherCommand(const std::vector<std::string> &fields) {  // can reply to the channel
            if (API_KEY.empty() || this->weather_sock == -1)
                return;

            if (fields.size() > 4)
            {
                std::map<std::string, std::string> weatherData;
                std::string response;
                std::string request = "GET /data/2.5/weather?q=" + fields[4] + "&appid=" + API_KEY + " HTTP/1.1\r\n";
                request += "Host: " HOST "\r\n";
                request += "Connection: close\r\n";

                weatherData["code"] = extractValue(response, "cod");
                if (weatherData["code"] == "401") {
                    
                    return;
                }

                weatherData["longitude"] = extractValue(response, "lon");
                weatherData["latitude"] = extractValue(response, "lat");
                weatherData["main"] = extractValue(response, "main");
                weatherData["description"] = extractValue(response, "description");
                weatherData["temperature"] = extractValue(response, "temp");
                weatherData["feels_like"] = extractValue(response, "feels_like");
                weatherData["temp_min"] = extractValue(response, "temp_min");
                weatherData["temp_max"] = extractValue(response, "temp_max");
                weatherData["pressure"] = extractValue(response, "pressure");
                weatherData["humidity"] = extractValue(response, "humidity");
                weatherData["visibility"] = extractValue(response, "visibility");
                weatherData["wind_speed"] = extractValue(response, "speed");
                weatherData["wind_deg"] = extractValue(response, "deg");
                weatherData["clouds"] = extractValue(response, "all");
                weatherData["country"] = extractValue(response, "country");
                weatherData["sunrise"] = extractValue(response, "sunrise");
                weatherData["sunset"] = extractValue(response, "sunset");
                weatherData["city"] = extractValue(response, "name");

            } else {
                // unavailble 
            }
        };

        void    pollCommand(const std::vector<std::string> &fields) {  // can reply to the channel
            // Creates and manages polls within the channel.
            // Collects votes and displays results.
        };

        void    helpCommand(const std::vector<std::string> &fields) { // can reply to the channel
            std::string command;
            std::string client = (fields[2] == BOT) ? fields[0] : fields[2];  // make a loop of 200ms delay between msgs

            command = "PRIVMSG " + client + " :Eny command marked with * can be used in a channel.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :-------------------------------------------------";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :/help                 - *Displays this help message.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :/joke                 - *Tells a random joke.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :/toss                 - *Tosses a coin: head or tails.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :/weather {city}       - *Gets weather info from the API for the specified city.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :/poll {option} {arg}  - *Poll command with options:";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :     - list                    : List all available polls.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :     - create {question}       : Create a new poll with the given question.";
            sendit(this->irc_sock, command);
            usleep(2000);
            command = "PRIVMSG " + client + " :     - vote {poll_id}          : Vote for a poll option.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :/admin {option} {arg} - Admin command with options:";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :     - add {user}        : Add a user to the master list.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :     - del {user}        : Remove a user from the master list.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :     - join {channel}    : Join the specified channel.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :     - part {channel}    : Leave the specified channel.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :     - list {option}     : display specific information.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :              - masters          : List all users with master privileges.";
            sendit(this->irc_sock, command);
            command = "PRIVMSG " + client + " :              - channels         : List all channels the bot is currently in.";
            sendit(this->irc_sock, command);
        };

        inline void    welcomeMsg(const std::vector<std::string> &fields) {
            std::string command = "PRIVMSG " + fields[0] + " :Welcome to " + fields[2] + " hope u like it here type /help in u need help."; // should be able to be set using admin
            sendit(this->irc_sock, command);
        };

        inline void    leaveMsg(const std::vector<std::string> &fields) {
            std::string command = "PRIVMSG " + fields[0] + " :hope i served u well come back soon " BOT; // should be able to be set using admin
            sendit(this->irc_sock, command);
        };

        inline void    pongCommand(const std::vector<std::string> &fields) {
            std::string command = "PONG " + fields[1];
            sendit(this->irc_sock, command);
        };

        bool    masterStatus(const std::string &name) {
            std::string command;

            for (const std::string &master : masters) {
                if (name == master)
                    return true;
            }
            command = "PRIVMSG " + name + " : u are not one of my masters, I cant help you.";
            sendit(this->irc_sock, command);
            return false;
        }

        void    adminCommand(const std::vector<std::string> &fields) {
            std::string command;
            std::vector<std::string>::iterator it;

            bool    sudo = false;

            if (!masterStatus(fields[0]))
                return;

            if (fields.size() > 5)
            {
                if (fields[4] == "join") {
                    if ((it = std::find(channels.begin(), channels.end(), fields[5])) == channels.end()) {
                        command = "JOIN " + fields[5];
                        channels.push_back(fields[5]);
                        sendit(this->irc_sock, command);
                    }
                } else if (fields[4] == "part") {
                    if ((it = std::find(channels.begin(), channels.end(), fields[5])) != channels.end()) {
                        command = "PART " + fields[5];
                        channels.erase(it);
                        sendit(this->irc_sock, command);
                    }
                } else if (fields[4] == "add") {
                    if ((it = std::find(channels.begin(), channels.end(), fields[5])) == channels.end()) {
                        masters.push_back(fields[5]);
                    }
                } else if (fields[4] == "del") {
                    if ((it = std::find(masters.begin(), masters.end(), fields[5])) != masters.end()) {
                        masters.erase(it);
                    }
                } else if (fields[4] == "set") {
                        API_KEY = fields[5];
                } else if (fields[4] == "list") {
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
                    command = "PRIVMSG " + fields[0] + " :" SYNXERROR;
                    sendit(this->irc_sock, command);
                }
            }
        };


        void startBot(uint16_t port, const char *password) {
            std::string answer;

            std::cout << "connect using :\n     1 - using LOCALHOST.\n     2 - using ip address.\n     3 - using domain name.\n" << std::endl; // hardcoded for now 
            std::cout << "Answer [1/2/3] ";
            if (getline(std::cin, answer) && answer == "1") {
                connectSockByIp(this->irc_sock, LOCALHOST, port);
            } else if (answer == "2") {
                std::cout << "Irc Server ip: ";
                getline(std::cin, answer);
                connectSockByIp(this->irc_sock, answer.c_str(), port);
            } else if (answer == "3") {
                std::cout << "Irc server name: "; 
                getline(std::cin, answer);
                if (!connectSockByName(this->irc_sock, answer.c_str(), port))
                    throw (std::exception());
            } else {
                std::cerr << "only [1/2/3] are allowed." << std::endl;
                throw (std::exception());
            }

            authenticate(password);

            if (connectSockByName(this->weather_sock, HOST, PORT)) {
                close(this->weather_sock);
                this->weather_sock = -1;
            }
            commandList["PING"] = &Bot::pongCommand;
            commandList["JOIN"] = &Bot::welcomeMsg;
            commandList["PART"] = &Bot::welcomeMsg;
            commandList["/help"] = &Bot::helpCommand;
            commandList["/joke"] = &Bot::jokeCommand;
            commandList["/toss"] = &Bot::tossCommand;
            commandList["/weather"] = &Bot::weatherCommand;
            // commandList["/poll"] = &Bot::;
            commandList["/admin"] = &Bot::adminCommand;

            listenForCommand();
        }

        void    unavailbleCommand(const std::vector<std::string> &fields) {
            std::string client = (fields[2] == BOT) ? fields[0] : fields[2];
            std::string commnad = "PRIVMSG " + client + " :" + fields[3] + "command unavailbe, contact a master";
        }

        ~Bot() {};
};

#endif