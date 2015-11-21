#include "Game/Net/ServerHost.hpp"
#include <RakSleep.h>
#include <GetTime.h>
#include <RakNetTime.h>
namespace cge
{
	
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

	ServerHost::ServerHost()
	{
		m_server = RakNet::RakPeerInterface::GetInstance();
	}

	RakNet::RakPeerInterface& ServerHost::getServer()
	{
		return *m_server;
	}

	ServerHost::~ServerHost( void )
	{
		m_server->Shutdown(500);
		RakNet::RakPeerInterface::DestroyInstance(m_server);
		m_server = NULL;
	}

	void ServerHost::run( int port )
	{
		RakNet::SocketDescriptor socketDescriptors;
		socketDescriptors.port=port;
		socketDescriptors.socketFamily=AF_INET; // Test out IPV4
		m_server->SetMaximumIncomingConnections(400);
		// Try again, but leave out IPV6
		bool b = m_server->Startup(400, &socketDescriptors, 1 )==RakNet::RAKNET_STARTED;
		if (!b)
		{
			puts("Server failed to start.  Terminating.");
			exit(1);
		}

		m_server->SetTimeoutTime(10000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);
		m_server->SetOccasionalPing(true);
		//m_server->SetUnreliableTimeout(1000);

		prerun(port);
		RakNet::TimeMS time0 = RakNet::GetTimeMS();
		while(true)
		{
			    doTick();
				runtick();
				RakSleep(30);

				RakNet::TimeMS time1 = RakNet::GetTimeMS();
				RakNet::TimeMS difference = time1 - time0;
				if(difference >= 1000)
				{
					time0 = RakNet::GetTimeMS();
					tick();
				}
		}
		postrun();
	}

	void ServerHost::doTick()
	{
		// Get a packet from either the server or the client

		for (RakNet::Packet* p = m_server->Receive(); p; m_server->DeallocatePacket(p), p=m_server->Receive())
		{
			// We got a packet, get the identifier with our handy function
			unsigned char packetIdentifier = GetPacketIdentifier(p);

			// Check if this is a network message packet
			switch (packetIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				m_peer.initServer(m_server,p->guid,p->systemAddress);
				onDisconnect(&m_peer);
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", p->guid);
				break;
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
				break;
			case ID_CONNECTION_BANNED: // Banned from this server
				printf("We are banned from this server.\n");
				break;			
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("Connection attempt failed\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
				break;

			case ID_INVALID_PASSWORD:
				printf("ID_INVALID_PASSWORD\n");
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST\n");
				m_peer.initServer(m_server,p->guid,p->systemAddress);
				onDisconnect(&m_peer);
				break;

			case ID_NEW_INCOMING_CONNECTION:
				// This tells the client they have connected
				m_peer.initServer(m_server,p->guid,p->systemAddress);
				onConnect(&m_peer);
				break;
			case ID_CONNECTED_PING:
			case ID_UNCONNECTED_PING:
				printf("Ping from %s\n", p->systemAddress.ToString(true));
				break;
			case ID_USER_PACKET_ENUM:
				{
					RakNet::RakString rs;
					RakNet::BitStream bsIn(p->data,p->length,false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(rs);
					m_peer.initServer(m_server,p->guid,p->systemAddress);
					onReceive(&m_peer,rs.C_String(),rs.GetLength());
				}
				break;
			default:
				break;
			}
		}
	}


}
