#ifndef __DEBRUINJ__
#define __DEBRUINJ__

#include "tree.h"

extern Symbol*  IVAR;
extern Symbol*  ILAMBDA;

Tree* 	abstract	(Tree* part, Tree* exp);			///< creates an ilambda by making variable <part> in <exp>

Tree*	ilambda		(Tree* body); 				   		///< a de bruijn lambda abstraction
Tree*	ivar		(int n);							///< a de bruijn variable of level n

bool	ilambdap 	(Tree* t, Tree** body);	   			///< true if t is an ilambda
bool	ivarp		(Tree* t, int* n);					///< true if t is an ivar

void	setLambdaProperty (Tree* t, Tree* v);	   		///< associate the variable part to a lambda term
Tree*	getLambdaProperty (Tree* t);				   	///< retreive the variable part associated to a lambda term 

#endif
