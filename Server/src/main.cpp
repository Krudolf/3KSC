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
#include "RakSleep.h"
#include "PacketLogger.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include "Kbhit.h"
#include <stdio.h>
#include <string.h>
#include "Gets.h"
#include <iostream>

#if LIBCAT_SECURITY==1
#include "SecureHandshake.h" // Include header for secure handshake
#endif

#if defined(_CONSOLE_2)
#include "Console2SampleIncludes.h"
#endif

// We copy this from Multiplayer.cpp to keep things all in one file for this example
unsigned char GetPacketIdentifier(RakNet::Packet *p);

#ifdef _CONSOLE_2
_CONSOLE_2_SetSystemProcessParams
#endif

int main(void)
{

	RakNet::RakPeerInterface *server=RakNet::RakPeerInterface::GetInstance();
	RakNet::RakNetStatistics *rss;
	server->SetIncomingPassword("Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));
	server->SetTimeoutTime(30000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	#if LIBCAT_SECURITY==1
		cat::EasyHandshake handshake;
		char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
		char private_key[cat::EasyHandshake::PRIVATE_KEY_BYTES];
		handshake.GenerateServerKey(public_key, private_key);
		server->InitializeSecurity(public_key, private_key, false);
		FILE *fp = fopen("publicKey.dat","wb");
		fwrite(public_key,sizeof(public_key),1,fp);
		fclose(fp);
	#endif

	// Holds packets
	RakNet::Packet* p;

	// GetPacketIdentifier returns this
	unsigned char packetIdentifier;

	// Record the first client that connects to us so we can pass it to the ping function
	RakNet::SystemAddress clientID=RakNet::UNASSIGNED_SYSTEM_ADDRESS;

	// Holds user data
	char portstring[30];

	// A server
	puts("Enter the server port to listen on");
	Gets(portstring,sizeof(portstring));
	if (portstring[0]==0)
		strcpy(portstring, "1234");
	
	puts("Starting server.");
	// Starting the server is very simple.  2 players allowed.
	// 0 means we don't care about a connectionValidationInteger, and false
	// for low priority threads
	// I am creating two socketDesciptors, to create two sockets. One using IPV6 and the other IPV4
	RakNet::SocketDescriptor socketDescriptors[2];
	socketDescriptors[0].port=atoi(portstring);
	socketDescriptors[0].socketFamily=AF_INET; // Test out IPV4
	socketDescriptors[1].port=atoi(portstring);
	socketDescriptors[1].socketFamily=AF_INET6; // Test out IPV6
	bool b = server->Startup(4, socketDescriptors, 2 )==RakNet::RAKNET_STARTED;
	server->SetMaximumIncomingConnections(4);
	if (!b)
	{
		printf("Failed to start dual IPV4 and IPV6 ports. Trying IPV4 only.\n");

		// Try again, but leave out IPV6
		b = server->Startup(4, socketDescriptors, 1 )==RakNet::RAKNET_STARTED;
		if (!b)
		{
			puts("Server failed to start.  Terminating.");
			exit(1);
		}
	}
	server->SetOccasionalPing(true);
	server->SetUnreliableTimeout(1000);

	DataStructures::List< RakNet::RakNetSocket2* > sockets;
	server->GetSockets(sockets);
	printf("Socket addresses used by RakNet:\n");
	for (unsigned int i=0; i < sockets.Size(); i++)
	{
		printf("%i. %s\n", i+1, sockets[i]->GetBoundAddress().ToString(true));
	}

	printf("\nMy IP addresses:\n");
	for (unsigned int i=0; i < server->GetNumberOfAddresses(); i++)
	{
		RakNet::SystemAddress sa = server->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS, i);
		printf("%i. %s (LAN=%i)\n", i+1, sa.ToString(false), sa.IsLANAddress());
	}

	printf("\nMy GUID is %s\n", server->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());
	puts("'quit' to quit. 'stat' to show stats. 'ping' to ping.\n'pingip' to ping an ip address\n'ban' to ban an IP from connecting.\n'kick to kick the first connected player.\n");
	char message[2048];
	int m_playerNumber = 0;
	std::string m_auxString;
	char const *t_connection;
	// Loop for input
	while (1)
	{

	// This sleep keeps RakNet responsive
		RakSleep(30);
		if (kbhit())
		{
			Gets(message,sizeof(message));

			if (strcmp(message, "quit")==0)
			{
				puts("Quitting.");
				break;
			}

			if (strcmp(message, "stat")==0)
			{
				rss=server->GetStatistics(server->GetSystemAddressFromIndex(0));
				StatisticsToString(rss, message, 2);
				printf("%s", message);
				printf("Ping %i\n", server->GetAveragePing(server->GetSystemAddressFromIndex(0)));
		
				continue;
			}

			if (strcmp(message, "ping")==0)
			{
				server->Ping(clientID);

				continue;
			}

			if (strcmp(message, "pingip")==0)
			{
				printf("Enter IP: ");
				Gets(message,sizeof(message));
				printf("Enter port: ");
				Gets(portstring,sizeof(portstring));
				if (portstring[0]==0)
					strcpy(portstring, "1234");
				server->Ping(message, atoi(portstring), false);

				continue;
			}

			if (strcmp(message, "kick")==0)
			{
				server->CloseConnection(clientID, true, 0);

				continue;
			}

			if (strcmp(message, "getconnectionlist")==0)
			{
				RakNet::SystemAddress systems[10];
				unsigned short numConnections=10;
				server->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
				for (int i=0; i < numConnections; i++)
				{
					printf("%i. %s\n", i+1, systems[i].ToString(true));
				}
				continue;
			}

			if (strcmp(message, "ban")==0)
			{
				printf("Enter IP to ban.  You can use * as a wildcard\n");
				Gets(message,sizeof(message));
				server->AddToBanList(message);
				printf("IP %s added to ban list.\n", message);

				continue;
			}

			if (strcmp(message, "reset")==0)
			{
				m_playerNumber = 0;
				continue;
			}



			// Message now holds what we want to broadcast
			char message2[2048];
			message2[0]=0;
			const static char prefix[] = "Server: ";
			strncpy(message2, prefix, sizeof(message2));
			strncat(message2, message, sizeof(message2) - strlen(prefix) - 1);
			server->Send(message2, (const int) strlen(message2)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}

		// Get a packet from either the server or the client
		for (p=server->Receive(); p; server->DeallocatePacket(p), p=server->Receive())
		{
			packetIdentifier = GetPacketIdentifier(p);
			switch (packetIdentifier)
			{
				case ID_DISCONNECTION_NOTIFICATION:
					printf("ID_DISCONNECTION_NOTIFICATION from %s\n", p->systemAddress.ToString(true));;
					break;

				case ID_NEW_INCOMING_CONNECTION:
					printf("ID_NEW_INCOMING_CONNECTION from %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
					clientID=p->systemAddress; // Record the player ID of the client
					if(m_playerNumber == 0)
					{
						m_auxString = "new:"+std::to_string(m_playerNumber);
						t_connection = m_auxString.c_str();  //use char const* as target type
						server->Send(t_connection, (int) strlen(t_connection)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
					}
					else if(m_playerNumber == 1)
					{
						m_auxString = "player:";
					 	t_connection = m_auxString.c_str();
					 	server->Send(t_connection, (int) strlen(t_connection)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
					}
					else
					{
						std::cout<<"-----==== SERVER FULL ===-----"<<std::endl;
						std::cout<<"Please type 'reset' to restart"<<std::endl;
					}
					++m_playerNumber;
					break;

				case ID_INCOMPATIBLE_PROTOCOL_VERSION:
					printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
					break;

				case ID_CONNECTED_PING:
				case ID_UNCONNECTED_PING:
					printf("Ping from %s\n", p->systemAddress.ToString(true));
					break;

				case ID_CONNECTION_LOST:
					printf("ID_CONNECTION_LOST from %s\n", p->systemAddress.ToString(true));;
					break;

				default:
					printf("%s\n", p->data);
					sprintf(message, "%s", p->data);
					server->Send(message, (const int) strlen(message)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
					break;
			}
		}
	}
	server->Shutdown(300);
	// We're done with the network
	RakNet::RakPeerInterface::DestroyInstance(server);
	return 0;
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
