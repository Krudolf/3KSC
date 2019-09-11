/*
 *  Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant 
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

// ----------------------------------------------------------------------
// RakNet version 1.0
// Filename ChatExample.cpp
// Very basic chat engine example
// ----------------------------------------------------------------------

#include "MessageIdentifiers.h"

#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "PacketLogger.h"
#include <assert.h>
#include <cstdio>
#include <stdlib.h>
#include "RakNetTypes.h"
#ifdef _WIN32
#include "Kbhit.h"
#include "WindowsIncludes.h" // Sleep
#else
#include "Kbhit.h"
#include <unistd.h> // usleep
#endif
#include "Gets.h"
#include "include/Client.hpp"
#include "include/entities/Arena.hpp"
#include "include/Game.hpp"
#include "include/managers/InputManager.hpp"


#if LIBCAT_SECURITY==1
#include "SecureHandshake.h" // Include header for secure handshake
#endif

unsigned char GetPacketIdentifier(RakNet::Packet *p);

Client& Client::instance(){
    static Client instance;
    return instance;
}

//Constructor
Client::Client(){
    m_inputManager  = &InputManager::instance();
    m_arena         = Arena::getInstance();
	m_game         	= Game::getInstance();
	m_arena->setOnline(true);
}

//Destructor
Client::~Client(){
    
}

void Client::send(char const *mens){
		//std::cout<<"Sending-> "<<mens<<std::endl;
	client->Send(mens, (int) strlen(mens)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::update(float p_time){
	listen();
	if(m_yourPlayer == 0)
	{
		Arena::getInstance()->onlineUpdate(p_time);
	}
	else
	{
		Arena::getInstance()->onlineUpdateClient(p_time);
	}
}
void Client::listen(){
	m_action = -1;
	// #ifdef _WIN32
	// 	Sleep(30);
	// #else
	// 	usleep(30 * 1000);
	// #endif
	char const * t_first = "";

	for (p=client->Receive(); p; client->DeallocatePacket(p), p=client->Receive())
	{
		packetIdentifier = GetPacketIdentifier(p);
		switch (packetIdentifier)
		{

		case ID_CONNECTION_LOST:
			printf("ID_CONNECTION_LOSTReceive\n");
			break;

		case ID_CONNECTION_REQUEST_ACCEPTED:
			t_first = "player";
			send(t_first);
			break;
		default:
			std::string t_messageReceived;
			t_messageReceived.append(reinterpret_cast<const char*>(p->data));
			readMessage(t_messageReceived);
			break;
		}
	}
}

void Client::start()
{
	m_yourPlayer = -1;
	client=RakNet::RakPeerInterface::GetInstance();
	m_flag = true;
	char t_debug[5];
	std::cout<<"Debug mode? (Y)/(N)"<<std::endl;
	std::cin >> t_debug;
	if (t_debug[0] == 'y' || t_debug[0] == 'Y')
		m_debug = true;

	RakNet::SystemAddress clientID=RakNet::UNASSIGNED_SYSTEM_ADDRESS;
	Gets(clientPort,sizeof(clientPort));
	if (clientPort[0]==0)
		strcpy(clientPort, "0");

	puts("Enter IP to connect to");
	Gets(ip, sizeof(ip));
	client->AllowConnectionResponseIPMigration(false);
	if (ip[0]==0)
		strcpy(ip, "127.0.0.1");
	
		
	puts("Enter the port to connect to");
	Gets(serverPort,sizeof(serverPort));
	if (serverPort[0]==0)
		strcpy(serverPort, "1234");

	char t_character[6];
	std::cout<<"Type your character number:\n1- Sparky\n2- Plup"<<std::endl;
	std::cin >> t_character;
	if (t_character[0] == '1')
		m_character = 1;
	else
		m_character = 0;
	std::cout<<"Character: "<<m_character<<std::endl;

	RakNet::SocketDescriptor socketDescriptor(atoi(clientPort),0);
	socketDescriptor.socketFamily=AF_INET;
	client->Startup(8,&socketDescriptor, 1);
	client->SetOccasionalPing(true);

	#if LIBCAT_SECURITY==1
		char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
		FILE *fp = fopen("publicKey.dat","rb");
		fread(public_key,sizeof(public_key),1,fp);
		fclose(fp);
	#endif

	#if LIBCAT_SECURITY==1
		RakNet::PublicKey pk;
		pk.remoteServerPublicKey=public_key;
		pk.publicKeyMode=RakNet::PKM_USE_KNOWN_PUBLIC_KEY;
		bool b = client->Connect(ip, atoi(serverPort), "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"), &pk)==RakNet::CONNECTION_ATTEMPT_STARTED;	
		#else
		RakNet::ConnectionAttemptResult car = client->Connect(ip, atoi(serverPort), "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"));
		RakAssert(car==RakNet::CONNECTION_ATTEMPT_STARTED);
	#endif
}

int Client::getActions(int p_player){
	return m_action;
}

unsigned char GetPacketIdentifier(RakNet::Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
		return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	}
	else
		return (unsigned char) p->data[0];
}

void Client::readMessage(std::string p_message){
	std::vector<std::string> t_parsed{explode(p_message, ':')};
	if(t_parsed[0] == "new"){
		m_yourPlayer = 0;
		m_yourPlayerString = "0";
		m_game->setChosenPlayer(m_yourPlayer, m_character);
		m_arena->spawnPlayerOnline(true, m_yourPlayer);

		if(m_debug) std::cout<<"Bienvenido, "<<t_parsed[1]<<" jugadores en la partida"<<std::endl;
		m_inputManager->setOnlineControl(m_yourPlayer);
	}
	if(t_parsed[0] == "second"){
		std::string t_toSend 	= "joined:" + std::to_string(m_character);
		char const *t_toSendChar = t_toSend.c_str();
		send(t_toSendChar);
		m_yourPlayer = 1;
		m_yourPlayerString = "1";
		m_game->setChosenPlayer(m_yourPlayer, m_character);
		m_game->setChosenPlayer(0, std::stoi(t_parsed[1]));
		m_arena->spawnPlayerOnline(true, 0);
		m_arena->spawnPlayerOnline(true, m_yourPlayer);
		m_inputManager->setOnlineControl(m_yourPlayer);
	}
	else if(t_parsed[0] == "player")
	{
		if(m_flag)
		{
			std::string t_toSend 	= "second:" + std::to_string(m_character);
			char const *t_toSendChar = t_toSend.c_str();
			send(t_toSendChar);
			m_flag = false;
		}
	}
	else if(t_parsed[0] == "joined"){
			m_game->setChosenPlayer(1, std::stoi(t_parsed[1]));
			m_arena->spawnPlayerOnline(true, 1);
	}
	else if(t_parsed[0] == "item"){
		Arena::getInstance()->spawnItemAt(std::stof(t_parsed[1]), std::stof(t_parsed[2]), std::stof(t_parsed[3]));
		if(m_debug) std::cout<<"Objeto de tipo "<<t_parsed[1]<<" añadido"<<std::endl;
	}
	else if(t_parsed[0] == "attack"){	
		int t_dmg = std::stoi(t_parsed[1]);
		bool t_block;
		if(std::stoi(t_parsed[2]) == 0)
			t_block = false;
		else
			t_block = true;
		float t_knockPower = std::stof(t_parsed[3]);
		int t_knockback = std::stoi(t_parsed[4]);
		int t_player = std::stoi(t_parsed[5]);
		Arena::getInstance()->getPlayer(t_player)->setX(std::stof(t_parsed[6]));
		Arena::getInstance()->getPlayer(t_player)->setY(std::stof(t_parsed[7]));
		Arena::getInstance()->getPlayer(t_player)->setVX(std::stof(t_parsed[8]));
		Arena::getInstance()->getPlayer(t_player)->receiveAttack(t_dmg, t_block, t_knockPower, t_knockback,true);
	}
	else if(t_parsed[0] == "portal"){
		Arena::getInstance()->spawnPortal();
	}
	else{
		if(t_parsed.size()<4)
		{
			//std::cout<<"MENSAJE NO IDENTIFICADO "<<t_parsed[0]<<std::endl;
			return;
		}
		Arena::getInstance()->getPlayer(std::stoi(t_parsed[0]))->setY(std::stof(t_parsed[2]));
		Arena::getInstance()->getPlayer(std::stoi(t_parsed[0]))->setX(std::stof(t_parsed[1]));
		Arena::getInstance()->getPlayer(std::stoi(t_parsed[0]))->setVX(std::stof(t_parsed[4]));
		bool t_actions[12];
		for(uint i = 0; i<12; i++)
		{
			if(t_parsed[3][i]=='0')
				t_actions[i] = false;
			else
				t_actions[i] = true;
		}
		m_inputManager->setNetPlayer(std::stoi(t_parsed[0]), t_actions);
		m_action = 0;

		if(m_debug)
		{
			std::cout<<"-- Recibiendo del servidor --"<<std::endl;
			std::cout<<"Jugador: "<<t_parsed[0]<<std::endl;
			std::cout<<"Posicion: ("<<t_parsed[1]<<","<<t_parsed[2]<<")"<<std::endl;
			printActions(t_parsed[3]);
			//printf("Ping: %i\n", client->GetLastPing(client->GetSystemAddressFromIndex(0)));
			std::cout<<""<<std::endl;
		}
	}
}

int Client::getPlayer(){
	return m_yourPlayer;
}

void Client::sendAction(bool p_actions[12]){
	float t_xPos = Arena::getInstance()->getPlayer(m_yourPlayer)->getX();
	float t_yPos = Arena::getInstance()->getPlayer(m_yourPlayer)->getY();
	float t_VX = Arena::getInstance()->getPlayer(m_yourPlayer)->getVX();
	std::string t_actions;
	for(uint i = 0; i < 12; i++)
	{
		if(p_actions[i])
			t_actions = t_actions + "1";
		else
			t_actions = t_actions + "0";
	}
    std::string t_toSend 	= m_yourPlayerString + ":" + std::to_string(t_xPos) + ":" + std::to_string(t_yPos) 
							+ ":" + t_actions + ":" + std::to_string(t_VX);
    char const *t_toSendChar = t_toSend.c_str();
	send(t_toSendChar);
	if(m_debug)
	{
		std::cout<<"-- Enviando al servidor --"<<std::endl;
		std::cout<<"ID: "<<m_yourPlayerString<<std::endl;
		std::cout<<"Posicion: ("<<t_xPos<<","<<t_yPos<<")"<<std::endl;
		printActions(t_actions);
		std::cout<<""<<std::endl;
	}
}

const std::vector<std::string> Client::explode(const std::string& s, const char& c)
{
	std::string buff{""};
	std::vector<std::string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

void Client::spawnItem(int p_type, int x, int y)
{
	std::string t_toSend 	= "item:" + std::to_string(p_type) + ":" + std::to_string(x) 
							+ ":" + std::to_string(y);
    char const *t_toSendChar = t_toSend.c_str();
	send(t_toSendChar);
}

void Client::spawnPortal()
{
	std::string t_toSend 	= "portal:1";
    char const *t_toSendChar = t_toSend.c_str();
	send(t_toSendChar);
}

void Client::printActions(std::string p_actions){
	std::cout<<"Acciones: "<<std::endl;
	if(p_actions[0]=='1')
		std::cout<<"     -Left"<<std::endl;
	if(p_actions[1]=='1')
		std::cout<<"     -Right"<<std::endl;
	if(p_actions[2]=='1')
		std::cout<<"     -Jump"<<std::endl;
	if(p_actions[3]=='1')
		std::cout<<"     -Run"<<std::endl;	
	if(p_actions[4]=='1')
		std::cout<<"     -Block"<<std::endl;
	if(p_actions[5]=='1')
		std::cout<<"     -Pick"<<std::endl;
	if(p_actions[6]=='1')
		std::cout<<"     -BasicAttack"<<std::endl;
	if(p_actions[7]=='1')
		std::cout<<"     -SpecialAttackUp"<<std::endl;	
	if(p_actions[8]=='1')
		std::cout<<"     -SpecialAttackDown"<<std::endl;
	if(p_actions[9]=='1')
		std::cout<<"     -SpecialAttackSide"<<std::endl;
	if(p_actions[10]=='1')
		std::cout<<"     -UltimateAttack"<<std::endl;
	if(p_actions[11]=='1')
		std::cout<<"     -Count"<<std::endl;	
	
	std::cout<<"Ping: "<<client->GetLastPing(client->GetSystemAddressFromIndex(0))<<std::endl;
}

void Client::attacked(int p_damage, bool p_block, float p_knockPower, int p_knockback)
{
	std::string t_toSend 	= "attack:" + std::to_string(p_damage) + ":" + std::to_string(p_block) + ":" + std::to_string(p_knockPower)
							+ ":" + std::to_string(p_knockback) + ":" + std::to_string(m_yourPlayer)
							+ ":" + std::to_string(Arena::getInstance()->getPlayer(m_yourPlayer)->getX())
							+ ":" + std::to_string(Arena::getInstance()->getPlayer(m_yourPlayer)->getY())
							+ ":" + std::to_string(Arena::getInstance()->getPlayer(m_yourPlayer)->getVX());
    char const *t_toSendChar = t_toSend.c_str();
	send(t_toSendChar);

	if(m_debug)
	{
		std::cout<<"-- Enviando al servidor --"<<std::endl;
		std::cout<<"ID: "<<m_yourPlayerString<<std::endl;
		std::cout<<"Daño recibido: "<< p_damage <<std::endl;
		std::cout<<"Bloqueable: "<< p_block <<std::endl;
		std::cout<<"Fuerza knockback: "<< p_knockPower <<std::endl;
		std::cout<<"Direccion fuerza: "<< p_knockback <<std::endl;
		std::cout<<"Mi X"<< std::to_string(Arena::getInstance()->getPlayer(m_yourPlayer)->getX())<<std::endl;
		std::cout<<"Mi Y"<< std::to_string(Arena::getInstance()->getPlayer(m_yourPlayer)->getY())<<std::endl;

		std::cout<<""<<std::endl;
	}
}