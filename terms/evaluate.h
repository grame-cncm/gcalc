#ifndef __EVALUATE__
#define __EVALUATE__

#include "tlib.h"

Tree* eval (Tree* e, Tree* env);	
Tree* apply (Tree* fun, Tree* arg);
Tree* search (Tree* env, int level);
Tree* force (Tree* v);
Tree* right (int dim, Tree* e);
Tree* left (int dim, Tree* e);

#endif
