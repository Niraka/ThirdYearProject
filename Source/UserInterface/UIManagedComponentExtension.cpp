#include "UIManagedComponentExtension.h"

UIManagedComponentExtension::UIManagedComponentExtension(UIExtendableComponent* c)
{
	c->registerExtension(this);
}