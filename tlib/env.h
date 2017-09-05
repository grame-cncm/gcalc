#ifndef __ENV__
#define __ENV__

#include "tree.h"

/**
 * An environment is a stack of Tree*. One can retrieve the nth element
 * from the top
 */
class Env
{
	Tree*	fElement;
	Env*	fNext;
	
 public:
	Env(Tree* t, Env* nxt) : fElement(t), fNext(nxt) {}
	
	Tree* 	retrieve(int n); ///< retrieve the nth element from the top
};

#endif
