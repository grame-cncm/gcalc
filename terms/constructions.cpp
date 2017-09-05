#include "constructions.h"
#include "cubes.h"

// construction constructor

Symbol*	CONSTRUCT 	= symbol("construct");
Symbol*	CLOSURE 	= symbol("closure");

// the various types of suspended computations
Symbol*	EVAL 		= symbol("eval");
Symbol*	APPLYRIGHT 	= symbol("apply.right");
Symbol*	APPLYLEFT 	= symbol("apply.left");
Symbol*	LEFT 		= symbol("left");
Symbol*	RIGHT 		= symbol("right");

// the environment
Symbol*	ENV 		= symbol("ENV");
Symbol*	NULLENV 	= symbol("EMPTY-ENV");

Tree* 	construct(int axe, Tree* l, Tree* r)	
{ 
	return tree(CONSTRUCT, tree(axe), l, r);
}

bool 	constructp (Tree* t, int* axe, Tree** l, Tree** r)	
{ 
	Tree* 	a;
	bool 	match = isTree(t, CONSTRUCT, &a, l, r);
	
	if (match) { *axe = a->getInt(); }
	return match; 
}


// constructors in currfied form
Tree* HorzPrimExp() { return abstract(BlueExp(), abstract(RedExp(), construct(0, BlueExp(), RedExp()))); }
Tree* VertPrimExp() { return abstract(BlueExp(), abstract(RedExp(), construct(1, BlueExp(), RedExp()))); }
Tree* ProfPrimExp() { return abstract(BlueExp(), abstract(RedExp(), construct(2, BlueExp(), RedExp()))); }

// construction operations in applicative form
Tree* HorzExp(Tree* e1, Tree* e2) { return application(application(HorzPrimExp(), e1), e2); }
Tree* VertExp(Tree* e1, Tree* e2) { return application(application(VertPrimExp(), e1), e2); }
Tree* ProfExp(Tree* e1, Tree* e2) { return application(application(ProfPrimExp(), e1), e2); }
