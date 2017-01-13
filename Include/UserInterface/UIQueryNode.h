/**
A UIQueryNode places a wrapper around a set of data that allows it to be retrieved by certain 
components on the user interface whenever it is added, removed or updated.

Each node has a name that, at the time of registration, must be unique in order to successfully register. 
This name is used to retrieve the node from the UIQueryManager whenever a UI component searches for it.
On top of this, each piece of data on a node must have a unique key that identifies the data. For example:
A UIText string that reads "<query=myNode,damage>" will grab a node named "myNode" and whatever data is
assigned to the key "damage".

It is strongly advised to use a UIManagedQueryNode instead of a UIQueryNode, as the managed version
contains all of the functionality within this class plus automatic (de)registration with the
UIQueryManager.

@author Nathan
*/

#ifndef UI_QUERY_NODE_H
#define UI_QUERY_NODE_H

#include <set>
#include <map>
#include <memory>
#include <string>

#include "Listenable.h"
#include "UIQueryNodeListener.h"

class UIQueryNode : public Listenable<UIQueryNodeListener*>
{
	public:
		/**
		Formatiting options for floating point numbers.
		Natural              : No formatting is applied
		One decimal place    : Value is rounded to one decimal place. Additional decimals removed.
		Two decimal places   : Value is rounded to two decimal place. Additional decimals  removed. (Default)
		Three decimal places : Value is rounded to three decimal places. Additional decimals removed.
		No trailing zeros    : Removes any zeros after the last non-zero value. */
		enum FloatStyle {NATURAL, ONE_DECIMAL_PLACE, TWO_DECIMAL_PLACE, THREE_DECIMAL_PLACE, NO_TRAILING_ZEROS};

	private:
		FloatStyle m_floatStyle;
		std::set<std::string> m_usedKeys;
		std::map<std::string, int> m_ints;
		std::map<std::string, bool> m_bools;
		std::map<std::string, float> m_floats;
		std::map<std::string, std::string> m_strings;

		/**
		Launches a data registered event to all registered UIQueryNodeListeners on this node.
		@param sKey The key of the node that was registered. */
		void launchEventDataRegistered(std::string sKey);

		/**
		Launches a data deregistered event to all registered UIQueryNodeListeners on this node.
		@param sKey The key of the node that was deregistered. */
		void launchEventDataDeregistered(std::string sKey);

		/**
		Launches a data modified event to all registered UIQueryNodeListeners on this node. 
		@param sKey The key of the node that was modified. */
		void launchEventDataModified(std::string sKey);

	protected:
		std::string m_sNodeName;

		/**
		Creates a UIQueryNode with the given name.
		@param sNodeName The name of the node. */
		UIQueryNode(std::string sNodeName);

		/**
		Destructor. */
		~UIQueryNode();
	
	public:
		/**
		Sets the formatting style to apply to floating point numbers when they are converted to strings via the
		toString() function. Any currently in-use floats are updated accordingly. Defaults to two decimal places.
		@param style The new FloatStyle. */
		void setFloatStyle(FloatStyle style);

		/**
		Returns a string representation of the data stored at the given key. Floating point numbers may be formatted depending
		on the current FloatStyle. If the key did not exist, an error string is returned instead.
		@param sKey The key of the data to get.
		@return A string representation of the data stored at the given key, or an error string if no such key existed. */
		std::string toString(std::string sKey);

		/**
		Sets the value of an integer registered with the given key. Interface components listening to this value are notified of the change.
		If either the new value was the same as the old value or the key did not exist, no action is performed.
		@param sKey The key of the data to set.
		@param iValue The new value. */
		void setQueryableInt(std::string sKey, int iValue);
	
		/**
		Sets the value of a boolean registered with the given key. Interface components listening to this value are notified of the change.
		If either the new value was the same as the old value or the key did not exist, no action is performed.
		@param sKey The key of the data to set.
		@param bValue The new value. */
		void setQueryableBool(std::string sKey, bool bValue);
	
		/**
		Sets the value of a float registered with the given key. Interface components listening to this value are notified of the change.
		If either the new value was the same as the old value or the key did not exist, no action is performed.
		@param sKey The key of the data to set.
		@param fValue The new value. */
		void setQueryableFloat(std::string sKey, float fValue);
	
		/**
		Sets the value of a string registered with the given key. Interface components listening to this value are notified of the change.
		If either the new value was the same as the old value or the key did not exist, no action is performed.
		@param sKey The key of the data to set.
		@param sValue The new value. */
		void setQueryableString(std::string sKey, std::string sValue);

		/**
		Modifies, through addition, an existing integer value stored at the given key. Interface components listening to this value are 
		notified of the change. If the modification was 0, no action is performed. 
		@param sKey The key of the data to modify. 
		@param iModification The value to add. */
		void modQueryableInt(std::string sKey, int iModification);

		/**
		Modifies an existing boolean value stored at the given key by inverting its value. Interface components listening to this value are 
		notified of the change. */
		void flipQueryableBool(std::string sKey);

		/**
		Modifies, through addition, an existing float value stored at the given key. Interface components listening to this value are
		notified of the change. If the modification was 0, no action is performed.
		@param sKey The key of the data to modify.
		@param fModification The value to add. */
		void modQueryableFloat(std::string sKey, float fModification);

		/**
		Returns the integer stored at the given key. Returns 0 if no such key existed. 
		@param sKey The key of the data to get. 
		@return The integer stored at the given key, or 0 if the key did not exist. */
		int getQueryableInt(std::string sKey);
		
		/**
		Returns the boolean stored at the given key. Returns false if no such key existed.
		@param sKey The key of the data to get.
		@return The boolean stored at the given key, or false if the key did not exist. */
		bool getQueryableBool(std::string sKey);
		
		/**
		Returns the float stored at the given key. Returns 0 if no such key existed.
		@param sKey The key of the data to get.
		@return The float stored at the given key, or 0 if the key did not exist. */
		float getQueryableFloat(std::string sKey);
		
		/**
		Returns the string stored at the given key. Returns an empty string if no such key existed.
		@param sKey The key of the data to get.
		@return The string stored at the given key, or an empty string if the key did not exist. */
		std::string getQueryableString(std::string sKey);

		/**
		Registers a new queryable integer with this node under the given key. The key must be unique to all data stored on
		the node, not just all integers. An initial value may be given.
		@param sKey The key of the data to register.
		@param iInitialValue The initial value of the integer. Defaults to 0. */
		void registerQueryableInt(std::string sKey, int iInitialValue = 0);
		
		/**
		Registers a new queryable boolean with this node under the given key. The key must be unique to all data stored on
		the node, not just all booleans. An initial value may be given.
		@param sKey The key of the data to register.
		@param bInitialValue The initial value of the boolean. Defaults to false. */
		void registerQueryableBool(std::string sKey, bool bInitialValue = false);
		
		/**
		Registers a new queryable float with this node under the given key. The key must be unique to all data stored on
		the node, not just all floats. An initial value may be given.
		@param sKey The key of the data to register.
		@param fInitialValue The initial value of the float. Defaults to 0. */
		void registerQueryableFloat(std::string sKey, float fInitialValue = 0.f);

		/**
		Registers a new queryable string with this node under the given key. The key must be unique to all data stored on
		the node, not just all strings. An initial value may be given.
		@param sKey The key of the data to register.
		@param sInitialValue The initial value of the string. Defaults to an empty string. */
		void registerQueryableString(std::string sKey, std::string sInitialValue = "");

		/**
		Deregisters the data stored at the given key. Any nodes using this data will be notified that the data has been removed. 
		If the key did not exist, no action is taken. 
		@param sKey The key of the data to deregister. */
		void deregisterQueryable(std::string sKey);
};

#endif