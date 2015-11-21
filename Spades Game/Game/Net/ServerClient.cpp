#include "Game/Net/ServerClient.hpp"
namespace cge
{
	unsigned char GetPacketIdentifierC(RakNet::Packet *p)
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

	ServerClient::ServerClient()
		:m_port(8099),m_connected(false),
		m_connecting(false),m_server(NULL),m_bgMode(false)
	{
		m_server = RakNet::RakPeerInterface::GetInstance();

		// Connecting the client is very simple.  0 means we don't care about
		// a connectionValidationInteger, and false for low priority threads
		RakNet::SocketDescriptor socketDescriptor(0,0);
		socketDescriptor.socketFamily=AF_INET;
		m_server->Startup(8,&socketDescriptor, 1);
		m_server->SetOccasionalPing(true);
		//m_server->SetTimeoutTime(15000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	}

	ServerClient::~ServerClient(void)
	{
        if(m_server)
        {
            m_server->Shutdown(200);
            RakNet::RakPeerInterface::DestroyInstance(m_server);
            m_server = NULL;
        }
	}

	ServerPeer* ServerClient::getClient()
	{
		return &m_client;
	}
    
    void ServerClient::setBGMode(bool on)
    {
        m_bgMode = on;
    }

	void ServerClient::tick()
	{
        if(m_server)
		for (RakNet::Packet* p = m_server->Receive(); p; m_server->DeallocatePacket(p), p=m_server->Receive())
		{
			// We got a packet, get the identifier with our handy function
			unsigned char packetIdentifier = GetPacketIdentifierC(p);
			//std::cout << (int)packetIdentifier << std::endl;
			// Check if this is a network message packet
            
            if(m_bgMode)
            {
                m_packets.push_back(ServerPacket());
                m_packets.back().packetID = packetIdentifier;
                for(int i = 0; i < p->length; i++)
                {
                    m_packets.back().message.push_back(p->data[i]);
                }
                continue;
            }
			switch (packetIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
					m_connected = false;
					m_connecting = false;
					onDisconnect();
					if(m_needsToConnect)
					{
						connect();
						m_needsToConnect = false;
					}
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", p->guid);
				onFailedToConnect(false);
				break;
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
				break;
			case ID_CONNECTION_BANNED: // Banned from this server
				printf("We are banned from this server.\n");
				break;			
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("Connection attempt failed\n");
				m_connecting = false;
				m_connected = false;
				onFailedToConnect(false);
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
				break;

			case ID_INVALID_PASSWORD:
				printf("ID_INVALID_PASSWORD\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
				m_connecting = false;
				m_connected = true;
				m_needsToConnect = false;
				m_client.initClient(m_server,p->guid,p->systemAddress);
				onConnect();
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
					onReceive(rs.C_String(),rs.GetLength());
				}
				break;
			default:
				break;
			}
		}
	}

	void ServerClient::send( const std::string& message )
	{
		if(needsToConnect())
		{
			queueMessage(message);
			return;
		}
		if(!connected() && !connecting())
			connect();

		if(connected())
		getClient()->write(message.c_str());
		else
			queueMessage(message);
	}

	int ServerClient::getPort() const
	{
		return m_port;
	}

	const std::string& ServerClient::getAddress() const
	{
		return m_address;
	}

	void ServerClient::setAddressAndPort( const std::string& address,int port )
	{
		m_address = address;
		m_port = port;
	}
    
    void ServerClient::execBG()
    {
        if(m_server)
            for (int i = 0; i < m_packets.size(); ++i)
            {
                // We got a packet, get the identifier with our handy function
                unsigned char packetIdentifier = m_packets[i].packetID;
                //std::cout << (int)packetIdentifier << std::endl;
                // Check if this is a network message packet
                switch (packetIdentifier)
                {
                    case ID_DISCONNECTION_NOTIFICATION:
                    case ID_CONNECTION_LOST:
                        // Connection lost normally
                        printf("ID_DISCONNECTION_NOTIFICATION\n");
                        m_connected = false;
                        m_connecting = false;
                        onDisconnect();
                        if(m_needsToConnect)
                        {
                            connect();
                            m_needsToConnect = false;
                        }
                        break;
                    case ID_ALREADY_CONNECTED:
                        // Connection lost normally
                        onFailedToConnect(false);
                        break;
                    case ID_INCOMPATIBLE_PROTOCOL_VERSION:
                        printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
                        break;
                    case ID_CONNECTION_BANNED: // Banned from this server
                        printf("We are banned from this server.\n");
                        break;
                    case ID_CONNECTION_ATTEMPT_FAILED:
                        printf("Connection attempt failed\n");
                        m_connecting = false;
                        m_connected = false;
                        onFailedToConnect(false);
                        break;
                    case ID_NO_FREE_INCOMING_CONNECTIONS:
                        // Sorry, the server is full.  I don't do anything here but
                        // A real app should tell the user
                        printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
                        break;
                        
                    case ID_INVALID_PASSWORD:
                        printf("ID_INVALID_PASSWORD\n");
                        break;
                    case ID_CONNECTION_REQUEST_ACCEPTED:
                        // This tells the client they have connected
                       // m_connecting = false;
                       // m_connected = true;
                        //m_needsToConnect = false;
                        //m_client.initClient(m_server,p->guid,p->systemAddress);
                       // onConnect();
                        break;
                    case ID_CONNECTED_PING:
                    case ID_UNCONNECTED_PING:
                       // printf("Ping from %s\n", p->systemAddress.ToString(true));
                        break;
                    case ID_USER_PACKET_ENUM:
                    {
                        RakNet::RakString rs;
                        RakNet::BitStream bsIn(&m_packets[i].message[0],m_packets[i].message.size(),false);
                        bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
                        bsIn.Read(rs);
                        onReceive(rs.C_String(),rs.GetLength());
                    }
                        break;
                    default:
                        break;
                }
            }
        
        m_packets.clear();
        m_bgMode = false;
    }

	bool ServerClient::connect()
	{
        if(m_server == NULL) {
            return false;
        }
        
		if(connected() && !connecting())
		{
			m_needsToConnect = true;
			return false;
		}

		if(connected() || connecting())
			return false;

		m_connecting = true;

		// Record the first client that connects to us so we can pass it to the ping function
		RakNet::SystemAddress clientID=RakNet::UNASSIGNED_SYSTEM_ADDRESS;

		RakNet::ConnectionAttemptResult car = m_server->Connect(m_address.c_str(), m_port, NULL, 0);
		RakAssert(car==RakNet::CONNECTION_ATTEMPT_STARTED);
		return true;
	}
    
    void ServerClient::regenerate() {
        if(m_server)
            destroy();
        m_server = RakNet::RakPeerInterface::GetInstance();
        
        // Connecting the client is very simple.  0 means we don't care about
        // a connectionValidationInteger, and false for low priority threads
        RakNet::SocketDescriptor socketDescriptor(0,0);
        socketDescriptor.socketFamily=AF_INET;
        m_server->Startup(8,&socketDescriptor, 1);
        m_server->SetOccasionalPing(true);
		m_server->SetTimeoutTime(10000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);
        onDisconnect();
    }
    
    void ServerClient::destroy() {
        m_connected = false;
        m_connecting = false;
        m_bgMode = false;
        m_packets.clear();
        
        m_server->Shutdown(200);
        RakNet::RakPeerInterface::DestroyInstance(m_server);
        m_server = NULL;
    }

	bool ServerClient::connected()
	{
		return m_connected;
	}

	bool ServerClient::connecting()
	{
		return m_connecting;
	}

	void ServerClient::harshDisconnect()
	{
		RakNet::BitStream temp( sizeof(unsigned char) );
		temp.Write( (RakNet::MessageID)ID_DISCONNECTION_NOTIFICATION );
		m_server->CloseConnection(m_client.getAddress(),true,0,IMMEDIATE_PRIORITY);
	}

	bool ServerClient::needsToConnect() const
	{
		return m_needsToConnect;
	}
    
    bool ServerClient::isBGExec() const
    {
        return m_bgMode;
    }

}
