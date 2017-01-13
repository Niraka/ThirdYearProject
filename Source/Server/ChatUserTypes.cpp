#include "ChatUserTypes.h"		

bool ChatUserTypes::isEqualOrGreater(Types type1, Types type2)
{
	return type1 <= type2;
}

bool ChatUserTypes::isGreater(Types type1, Types type2)
{
	return type1 < type2;
}

bool ChatUserTypes::isEqualOrLess(Types type1, Types type2)
{
	return type1 >= type2;
}

bool ChatUserTypes::isLess(Types type1, Types type2)
{
	return type1 > type2;
}

bool ChatUserTypes::isEqual(Types type1, Types type2)
{
	return type1 == type2;
}