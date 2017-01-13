#include "ChatMessageValidator.h"

bool ChatMessageValidator::isMessageValid(const std::string& sMessage)
{
	return !sMessage.empty();
}