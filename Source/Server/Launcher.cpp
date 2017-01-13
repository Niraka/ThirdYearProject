#include "Server.h"
#include <iostream>

int main()
{
	Server r;
	if (r.init())
	{
		r.run();
	}
	else
	{
		std::cout << "Server initialisation failed. Press any key to close" << std::endl;
		std::string sLine;
		getline(std::cin, sLine);
	}
}