#include "application.h"

Symbol* APPL = symbol("appl");

Tree* application( Tree* fun, Tree* arg)
{
	return tree(APPL, fun, arg);
}


bool applicationp (Tree* t, Tree** fun, Tree** arg)
{
	return isTree(t, APPL, fun, arg);
}
