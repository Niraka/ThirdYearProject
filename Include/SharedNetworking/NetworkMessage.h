/**
@author Nathan
*/

#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

#include <SFML/Network/Packet.hpp>
#include "NetworkInstructionDefinitions.h"

struct NetworkMessage
{
	sf::Int32 iNetworkId;
	NetworkInstructionIds instructionId;
	sf::Packet packet;

	NetworkMessage();
};

#endif