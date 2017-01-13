#ifndef CHAT_MESSAGE_VALIDATOR_H
#define CHAT_MESSAGE_VALIDATOR_H

#include <string>

class ChatMessageValidator
{
	private:
	protected:
	public:
		/**
		Verifies whether a message that is about to be broadcast meets a series of criteria for
		being a valid message.
		@param sMessage The message to broadcast
		@return True if the message was valid, false otherwise */
		bool isMessageValid(const std::string& sMessage);
};

#endif