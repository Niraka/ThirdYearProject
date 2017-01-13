/**
The ChatService is responsible for owning and managing chat participants and the channels
that they are active within.

Chat is divided in to two categories: One-to-one and one-to-many. OTM uses channels, OTO
does not.

The loading of user information is not handled here and should be performed elsewhere prior
to adding a user. In particular, the adding and removing of friends and enemies is handled
by the character information management component of the server: NOT the chat service.

@author Nathan */

#ifndef CHAT_SERVICE_H
#define CHAT_SERVICE_H

#include "IndexedVector.h"
#include "ChatChannel.h"

class ChatService
{
	private:
		/* A log of all messages sent over this channel that havent been written to disk yet.
		This includes messages that were rejected due to muting or an illegal message */
		std::queue<std::string> m_messageLog;

		/* A message validation object */
		ChatMessageValidator m_chatMessageValidator;

		/* A string that is prepended to a successfully sent message in the internal message log */
		std::string m_sSentString;

		/* A string that is prepended to a rejected message in the internal message log */
		std::string m_sFailString;

		/* A pointer to the NetworkManager with which to send messages through */
		NetworkManager* m_networkManager;

		/* The chat channels. Indexes are equivalent to their respective ids */
		IndexedVector<ChatChannel> m_channels;

		/* The users. Indexes are equivalent to their respective ids */
		IndexedVector<ChatUserInfo> m_users;

		/**
		Resets the channels to the default state. */
		void resetChannels();

	protected:

	public:
		ChatService();
		~ChatService();

		unsigned int getIdOfChannnel(const std::string& sChannelName);

		/**
		Adds a user. The user is allocated a unique id.
		@param info The chat user information for this user
		@return The id of the user */
		unsigned int addUser(const ChatUserInfo& info);

		/**
		Removes a user with the given id. The user is ejected from all channels they are
		currently a member of. If no such user existed, no action is taken. 
		@param uiUserId The id of the user to remove 
		@return True if a user was removed, false otherwise */
		bool removeUser(unsigned int uiUserId);

		/**
		Queries the existence of a user with the given id.
		@param uiUserId The id of the user to search for
		@return True if the user existed, false otherwise */
		bool userExists(unsigned int uiUserId);

		/**
		Creates a channel. Names do not need to be unique and are only used to identify the channel 
		type in a log file.
		@param sChannelName The name of the channel 
		@return The id of the channel */
		unsigned int createChannel(const std::string& sChannelName);

		/**
		Deletes a channel with the given id. If no such channel existed, no action is taken. 
		@param uiChannelId The id of the channel to delete
		@return True if a channel was deleted, false otherwise */
		bool deleteChannel(unsigned int uiChannelId);

		/**
		Queries the existence of a channel with the given id. 
		@param uiChannelId The id of the channel to search for
		@return True if the channel exists, false if it does not */
		bool channelExists(unsigned int uiChannelId);

		/**
		Retrieves a copy of the message log of the target channel. Returns an empty queue if
		the channel did not exist.
		@param uiChannelId The id of the channel whose log to retrieve 
		@return A copy of the target channels message log */
		std::queue<std::string> getChannelLog(unsigned int uiChannelId);

		/**
		Retrieves a copy of the direct messaging message log. 
		@return A copy of the direct messaging message log */
		std::queue<std::string> getLog();

		/*
		Clears the internal message log of the target channel. 
		@param uiChannelId The id of the channel whose log to clear 
		@return True if a channel was successfully accessed, false otherwise */
		bool clearChannelLog(unsigned int uiChannelId);

		/**
		Clears the direct messaging log. */
		void clearLog();

		/**
		Attempts to add a user to a channel. This function will fail (and take no action) if the user
		or channel did not exist. It will also fail and return false if the user was already a member
		of the channel.
		@param uiUserId The id of the user to add
		@param uiChannelId The id of the channel 
		@return True if the user was added, false otherwise */
		bool addUserToChannel(unsigned int uiUserId, unsigned int uiChannelId);

		/**
		Attempts to remove a user from a channel. If the channel, user, or users presence within the
		channel did not exist, this function does nothing.
		@param uiUserId The id of the user to remove
		@param uiChannelId The id of the channel
		@return True if a user from removed, false otherwise */
		bool removeUserFromChannel(unsigned int uiUserId, unsigned int uiChannelId);
		
		/**
		Sets the muted status of a user with the given id.
		@param uiUserId The id of the user 
		@param bMuted The muted status 
		@return True if the user was successfully accessed, false otherwise */
		bool setUserMuted(unsigned int uiUserId, bool bMuted);

		/**
		Sets the user type of a user with the given id.
		@param uiUserId The id of the user
		@param type The new user type
		@return True if the user was successfully accessed, false otherwise */
		bool setUserType(unsigned int uiUserId, ChatUserTypes::Types type);

		/**
		Sets the minimum user type to be able to speak in a channel. Defaults to SPEAKER.
		@param uiChannelId The id of the channel to modify
		@param type The type
		@return True if a channel was successfully accessed, false otherwise */
		bool setChannelSpeakerType(unsigned int uiChannelId, ChatUserTypes::Types type);

		/**
		Enables or disables the pre-pending of usernames to messages sent via the target channel.
		Defaults to enabled.
		@param uiChannelId The id of the channel
		@param bEnabled True to enable, false to disable 
		@return True if a channel was successfully accessed, false otherwise */
		bool setUserDescriptorsEnabled(unsigned int uiChannelId, bool bEnabled);

		/**
		Attempts to broadcast a message on the target channel as the given user. This function
		will fail if the user does not have permission to speak or the  user/channel ids were invalid.
		Some messages also cause a failure.
		@param uiSenderId The id of the sender
		@param uiChannelId The id of the channel to broadcast on
		@param sMessage The message to broadcast
		@return True if the message successfully sent, false otherwise */
		bool broadcast(unsigned int uiSenderId, unsigned int uiChannelId, const std::string& sMessage);

		/**
		Attempts to send a message to a user as the given user. This function will fail if the
		message is illegal, the sender id is invalid, the target user did not exist or the target
		has blocked the sender.
		@param uiSenderId The id of the sender
		@param sTarget The name of the person to send to
		@param sMessage The message to send
		@return True if the message was successfully sent, false otherwise */
		bool send(unsigned int uiSenderId, const std::string& sTarget, const std::string& sMessage);
};

#endif