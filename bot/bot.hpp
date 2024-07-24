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

    private:
        void            logger(const std::string &message) const;
        bool            masterStatus(const std::string &name) const;

        inline void     pongCommand(const std::vector<std::string> &fields);
        inline void     welcomeMsg(const std::vector<std::string> &fields);
        inline void     leaveMsg(const std::vector<std::string> &fields);
        void            helpCommand(const std::vector<std::string> &fields);
        void            jokeCommand(const std::vector<std::string> &fields);
        void            tossCommand(const std::vector<std::string> &fields);
        void            weatherCommand(const std::vector<std::string> &fields);
        // void         pollCommand(const std::vector<std::string> &fields);
        void            adminCommand(const std::vector<std::string> &fields);

        const std::map<std::string, std::string>    extractWeatherData(const std::string &response);
        std::string                                 getWeatherData(const std::map<std::string, std::string>& weatherData , const std::string &client);


    public:
        Bot(void);

        void startBot(uint16_t port);
        void authenticate(std::string password) const;
        void listenForCommand(void);

        ~Bot() {};
};

#endif