/**
A ModelSet holds information about a model set file.

The set is used by the ModelLoader to know which models to unload from memory when you wish to
unload a set of models from memory. The alternative solution would be to read the model set file 
a second time to determine the names of the models to unload.

@author Nathan 
*/

#ifndef MODEL_SET_H
#define MODEL_SET_H

#include <string>
#include <vector>

struct ModelSet
{
	std::vector<std::string> sModelNames;
};

#endif