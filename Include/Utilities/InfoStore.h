#ifndef INFO_STORE_H
#define INFO_STORE_H

#include <string>
#include <vector>

class InfoStore
{
	private:
		std::vector<std::string> m_info;
		std::vector<std::string> m_errors;

	protected:
		/**
		Clears the error info stored on the object. */
		void clearErrorInfo();

		/**
		Clears the info stored on the object. */
		void clearInfo();

		/**
		Appends a string to the objects information store. 
		@param sInfo The info to append */
		void addInfo(std::string sErr);

		/**
		Appends a string to the objects error information store.
		@param sError The error to append */
		void addErrorInfo(std::string sError);

	public:
		InfoStore();
		~InfoStore();

		/**
		Retrieves the error information stored on the object. 
		@return The error information store */
		const std::vector<std::string>& getErrorInfo();

		/**
		Retrieves the information stored on the object.
		@return The information store */
		const std::vector<std::string>& getInfo();
};

#endif