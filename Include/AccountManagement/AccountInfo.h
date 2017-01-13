/**
@author Nathan */

#ifndef AM_ACCOUNT_INFO_H
#define AM_ACCOUNT_INFO_H

#include <string>
#include "AccountProperties.h"

struct AccountInfo
{
	/* The name of the account */
	std::string sAccountName;

	/* The type of account */
	AccountTypes accountType;

	/* The behaviour state of the account */
	BehaviourState behaviourState;
};

#endif