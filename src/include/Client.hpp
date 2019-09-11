#ifndef CLIENT
#define CLIENT

class InputManager;
class Arena;
class Game;

#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include <iostream>
#include <vector>

class Client{
    public:
        static Client& instance();
        Client();
        ~Client();
        void start();
        void update(float p_time);
        void listen();
        void send(char const *mens);
        int getActions(int p_player);
        void readMessage(std::string p_message);
        int getPlayer();
        void sendAction(bool p_actions[12]);
        const std::vector<std::string> explode(const std::string& s, const char& c);
        void spawnItem(int p_type, int x, int y);
        void spawnPortal();
        void printActions(std::string p_actions);
        void attacked(int p_damage, bool p_block, float p_knockPower,int p_knockback);
        
    private:
        InputManager*   m_inputManager;
        Arena*          m_arena;
        Game*           m_game;

        RakNet::RakNetStatistics *rss;
        RakNet::RakPeerInterface *client;
	    RakNet::Packet* p;
        unsigned char packetIdentifier;
        char message[2048];
        RakNet::SocketDescriptor socketDescriptor;
        char ip[64], serverPort[30], clientPort[30];
        int m_action;
        int m_yourPlayer;
        bool m_debug = false;
        int m_character;
        bool m_flag;
        std::string m_yourPlayerString;
};

#endif