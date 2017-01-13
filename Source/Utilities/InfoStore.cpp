#include "InfoStore.h"

InfoStore::InfoStore()
{
}

InfoStore::~InfoStore()
{
}

void InfoStore::clearInfo()
{
	m_info.clear();
}

void InfoStore::clearErrorInfo()
{
	m_errors.clear();
}

void InfoStore::addInfo(std::string sInfo)
{
	m_info.push_back(sInfo);
}

void InfoStore::addErrorInfo(std::string sError)
{
	m_errors.push_back(sError);
}

const std::vector<std::string>& InfoStore::getInfo()
{
	return m_info;
}

const std::vector<std::string>& InfoStore::getErrorInfo()
{
	return m_errors;
}