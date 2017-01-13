#include "Client.h"

int main()
{
	std::shared_ptr<Client> r = std::make_shared<Client>();
	r->init();
	r->run();
	
	return 0;
}