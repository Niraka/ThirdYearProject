#include "Account.h"

Account::Account()
{
	m_iNetworkId = -1;
}

Account::Account(const std::string& sAccountName,sf::Int32 iNetworkId, AccountData& data)
{
	m_sAccountName = sAccountName;
	m_iNetworkId = iNetworkId;
	m_data = data;
}

Account::~Account()
{
}

const std::string& Account::getName() const
{
	return m_sAccountName;
}

const sf::Int32 Account::getNetworkId() const
{
	return m_iNetworkId;
}

AccountData& Account::getData()
{
	return m_data;
}