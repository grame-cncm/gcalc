#ifndef __CONSTRUCTIONS__
#define __CONSTRUCTIONS__

#include "tlib.h"


extern Symbol*	CONSTRUCT;
extern Symbol*	CLOSURE;

// the various types of suspended computations
extern Symbol*	EVAL;
extern Symbol*	APPLYRIGHT;
extern Symbol*	APPLYLEFT;
extern Symbol*	LEFT;
extern Symbol*	RIGHT;

// the environment
extern Symbol*	ENV;
extern Symbol*	NULLENV;

// constructor and predicat for constructions

Tree* 	construct  (int axe, Tree* l, Tree* r);					///< assemble two expressions	
bool 	constructp (Tree* t, int* axe, Tree** l, Tree** r);		///< test if t is a construct

// constructors in curryfied form
Tree* HorzPrimExp();
Tree* VertPrimExp();
Tree* ProfPrimExp();

// construction operations in applicative form
Tree* HorzExp(Tree* e1, Tree* e2);
Tree* VertExp(Tree* e1, Tree* e2);
Tree* ProfExp(Tree* e1, Tree* e2);

// name property
void	setNameProperty(Tree* t, const char* name);
const char* getNameProperty (Tree* t);


#endif
