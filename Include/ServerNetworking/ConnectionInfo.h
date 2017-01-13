/**
A ConnectionInfo contains information about a connection.

'handler' is a pointer to the NetworkHandler that messages arriving on this connection will be handed to. If 
no handler has been set, the default handler is used instead. If no default handler could be retrieved, the
connection is non-functional and will discard incoming messages.

'ipAddress' is the IP Address of the client. This could be a public address, a local address or the loopback
address. Defaults to an invalid address.

'socket' is the TCP socket associated with the connection.

'iRemotePort' is the port of the client that the connection is communicating over. Default to 0.

@author Nathan
*/


#ifndef NM_CONNECTION_INFO_H
#define NM_CONNECTION_INFO_H

#include <string>
#include <SFML\Network\IpAddress.hpp>
#include <SFML\Network\TcpSocket.hpp>

#include "NetworkHandler.h"

struct ConnectionInfo
{
	NetworkHandler* handler;
	sf::TcpSocket* socket;
	sf::IpAddress ipAddress;
	unsigned short iRemotePort;

	ConnectionInfo();
	ConnectionInfo(unsigned short port, sf::IpAddress ip, sf::TcpSocket* s, NetworkHandler* h);
};

#endif