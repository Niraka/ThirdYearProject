/**
The RealmListener is an interface that provides the necessary function definitions for a class to be
able to register to listen to events occurring on Realm objects.

This is mostly intended to be used as a way of simply knowing when to resend realm data to accounts who
are idling no the realm selection screen.

@author Nathan */

#ifndef REALM_LISTENER_H
#define REALM_LISTENER_H

#include <string>

class RealmListener
{
	private:

	protected:

	public:
		/**
		This function is called when a property on a Realm this RealmListener is listening to is changed.
		@param sRealmName The name of the Realm whose property changed */
		virtual void onRealmPropertyChanged(const std::string& sRealmName) = 0;

		/**
		This function is called when an AccountSession on a Realm this RealmListener is listening to
		requests that it be logged out. 
		@param sRealmName The name of the Realm whose property changed 
		@param iNetworkId The network id of the AccountSession to logout */
		virtual void onRealmLogoutRequested(const std::string& sRealmName, const sf::Int32 iNetworkId) = 0;
};

#endif