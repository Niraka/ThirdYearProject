/**
A structure for storing information about a zone transfer request.

@author Nathan */

#ifndef ZONE_TRANSFER_REQUEST_H
#define ZONE_TRANSFER_REQUEST_H

#include "PlayerInfo.h"

struct ZoneTransferRequest
{
	PlayerInfo* info;
	std::string sTargetZone;
	int iTargetPoint;
};

#endif