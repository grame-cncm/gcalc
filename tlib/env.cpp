#include "env.h"



/**
 * Retrieve the nth element from the top of an environment
 * Warning : no test are made, we suppose the nth element exist
 */
Tree* Env::retrieve (int n) 
{ 
	Env* e=this; 
	while (n>0) { n = n-1; e = e->fNext; }
	return e->fElement;
}
