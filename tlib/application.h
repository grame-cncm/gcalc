#ifndef __APPLICATION__
#define __APPLICATION__

#include "tree.h"

extern Symbol* APPL;

Tree* application( Tree* fun, Tree* arg);
bool  applicationp (Tree* t, Tree** fun, Tree** arg);

#endif
