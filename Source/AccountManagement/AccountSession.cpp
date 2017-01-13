#include "AccountSession.h"

AccountSession::AccountSession()
{
}

AccountSession::AccountSession(Account* account)
{
	m_account = account;
	m_iNetworkId = m_account->getNetworkId();
}

const std::string& AccountSession::getAccountName() const
{
	return m_account->getName();
}

const sf::Int32 AccountSession::getNetworkId() const
{
	return m_iNetworkId;
}

const AccountData& AccountSession::getAccountData() const
{
	return m_account->getData();
}