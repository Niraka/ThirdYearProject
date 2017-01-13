#include "ConnectionInfo.h"

ConnectionInfo::ConnectionInfo()
{
	handler = nullptr;
	socket = nullptr;
	ipAddress = sf::IpAddress::None;
	iRemotePort = 0;
}

ConnectionInfo::ConnectionInfo(unsigned short port, sf::IpAddress ip, sf::TcpSocket* s, NetworkHandler* h)
{
	handler = h;
	socket = s;
	ipAddress = ip;
	iRemotePort = port;
}