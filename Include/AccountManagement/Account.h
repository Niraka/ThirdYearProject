/**
An Account object is responsible for storing information about a user who is currently connected to the
server. 

The data for an account is loaded externally and is bound during the construction of the Account object
so that the Accounts themselves do not need to be concerned by the method in which data is loaded.

@author Nathan */

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <SFML\Network\Packet.hpp>
#include "AccountData.h"

class Account
{
	private:
		sf::Int32 m_iNetworkId;
		AccountData m_data;
		std::string m_sAccountName;

		Account();

	protected:

	public:
		/**
		Creates a new Account and stores it with the name, id and data.
		@param sAccountName The name of the account
		@param iNetworkId The network id
		@param data The accounts data */
		Account(const std::string& sAccountName, sf::Int32 iNetworkId, AccountData& data);

		~Account();

		/**
		Retrieves the account name. 
		@return The account name */
		const std::string& getName() const;

		/**
		Retrieves the network id of the Account.
		@return The network id */
		const sf::Int32 getNetworkId() const;

		/**
		Retrieves an editable reference to the accounts data. 
		@return The AccountData */
		AccountData& getData();
};

#endif 