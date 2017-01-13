/**
A ChatChannel handles one particular channel of communication. It may contain any number of
users.

@author Nathan */

#ifndef CHAT_CHANNEL_H
#define CHAT_CHANNEL_H

#include <queue>
#include <string>

#include "ChatUserInfo.h"
#include "NetworkManager.h"
#include "SearchableVector.h"
#include "ChatMessageValidator.h"

class ChatChannel
{
	private:
		/* A log of all messages sent over this channel that havent been written to disk yet.
		This includes messages that were rejected due to muting or an illegal message */
		std::queue<std::string> m_messageLog;

		/* A listing of the users currently within this channel */
		SearchableVector<ChatUserInfo*> m_users;

		/** The name of the channel */
		std::string m_sChannelName;

		/* The unique id of the channel */
		unsigned int m_uiChannelId;

		/* The flag for prepending or not prepending usernames to messages */
		bool m_bUserDescriptorsEnabled;

		/* A pointer to the NetworkManager with which to send messages through */
		NetworkManager* m_networkManager;

		/* A string that is prepended to a successfully sent message in the internal message log */
		std::string m_sSentString;

		/* A string that is prepended to a rejected message in the internal message log */
		std::string m_sFailString;

		/* A chat user types object for user type comparisons */
		ChatUserTypes m_chatUserTypes;

		/* The minimum required user type to be able to speak in this channel */
		ChatUserTypes::Types m_speakUserType;

		/* A message validation object */
		ChatMessageValidator m_chatMessageValidator;

	protected:

	public:
		ChatChannel();

		/**
		Constructs a ChatChannel with the given name. Names do not need to be unique and 
		are only used to identify the channel type in a log file.
		@param sChannelName The name of the channel */
		ChatChannel(std::string sChannelName);

		~ChatChannel();

		/**
		Gets the name of the channel.
		@return The name of the channel */
		const std::string& getName() const;

		/**
		Retrieves a reference to the channel message log. 
		@return A reference to the channel message log */
		std::queue<std::string>& getLog();

		/*
		Clears the internal message log. */
		void clearLog();

		/**
		Enables or disables the pre-pending of usernames to messages. Defaults to enabled. 
		@param bEnabled True to enable, false to disable */
		void setUserDescriptorsEnabled(bool bEnabled);

		/**
		Sets the channel id. This should be unique across all other channels.  
		@param uiChannelId */
		void setChannelId(unsigned int uiChannelId);

		/**
		Sets the minimum user type to be able to speak in this channel. Defaults to SPEAKER.
		@param type The type */
		void setSpeakerType(ChatUserTypes::Types type);

		/**
		Attempts to add a user. 
		@param info The information relating to the user to add
		@return True if the user was successfully added, false otherwise */
		bool addUser(ChatUserInfo* info);

		/**
		Attempts to remove a user. If the user did not exist, no action is taken.
		@param info The information relating to the user to remove
		@return True if the user was successfully removed, false otherwise */
		bool removeUser(ChatUserInfo* info);

		/**
		Queries the existence of a user within this channel. 
		@param info The information relating to the user to search for
		@return True if the user existed, false otherwise */
		bool userExists(ChatUserInfo* info);

		/**
		Attempts to broadcast a message as the given user. This function will fail if the user 
		does not have permission to speak or the user id was invalid. Some message types also 
		cause a failure.
		@param uiSenderId The id of the sender
		@param sMessage The message to broadcast
		@return True if the message successfully sent, false otherwise */
		bool broadcast(unsigned int uiSenderId, const std::string& sMessage);
};

#endif