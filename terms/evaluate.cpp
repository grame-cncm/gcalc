#include "evaluate.h"
#include "cubes.h"
#include "constructions.h"
#include <iostream>

using namespace std;

// suspendEval : evaluate expression <e> in environment <env> up to 
// the first application
		 
static Tree* suspendEval(Tree* e, Tree* env)	
{
	int		rgba;
	int 	dim, level;
	Tree 	*left, *right, *body;
	
		
	if (e->isa(IVAR, &level)) {

		return search(env, level);

	} else if (e->isa(CONSTRUCT, &dim, &left, &right)) {

		return tree(CONSTRUCT, dim, suspendEval(left,env),suspendEval(right,env));

	} else if (e->isa(CUBE, &rgba)) {

		return e;

	} else if (e->isa(ILAMBDA, &body)) {

		return tree(CLOSURE,e,env);

	} else {
		
		return tree(EVAL,e,env);
		//cout << "ERROR : not evaluable : " << *e << endl;
		//return tree(CUBE, 0x888888FF);
	} 
}
		 
Tree* eval(Tree* e, Tree* env)	
{
	int		rgba;
	int 	dim, level;
	Tree 	*left, *right, *fun, *arg, *body;
	
		
	if (e->isa(IVAR, &level)) {

		return force(search(env, level));
		//return search(env, level);

	} else if (e->isa(APPL, &fun, &arg)) {

		return apply(eval(fun,env), suspendEval(arg,env));

	} else if (e->isa(CONSTRUCT, &dim, &left, &right)) {

		return tree(CONSTRUCT, dim, suspendEval(left,env), suspendEval(right,env));

	} else if (e->isa(CUBE, &rgba)) {

		return e;

	} else if (e->isa(ILAMBDA, &body)) {

		return tree(CLOSURE,e,env);

	} else {

		cout << "ERROR : not evaluable : " << *e << endl;
		return tree(CUBE, 0x888888FF);
	} 
}


// apply function <fun> to argument <arg>
Tree* apply(Tree* fun, Tree* arg)
{
	Tree 	*lam, *env, *left, *right;
	int		dim;

	fun = force(fun);
	
	if (fun->isa(CLOSURE,&lam,&env)) {
		
		Tree *body; lam->isa(ILAMBDA,&body);
		return eval(body,tree(ENV,arg,env));

	} else if (fun->isa(CONSTRUCT, &dim, &left, &right)) {

		return tree(CONSTRUCT, dim, tree(APPLYLEFT,dim,left,arg), tree(APPLYRIGHT,dim,right,arg));

	} else {

		return tree(APPL,fun,arg);
	}
}


// forces a suspended computation
Tree* oldforce(Tree* e)
{
	int		dim;
	Tree	*exp, *env, *fun, *arg;
	
	if (e->isa(EVAL,&exp,&env)) {
		
		return eval(exp,env);
		
	} else if (e->isa(APPLYLEFT,&dim,&fun,&arg)) {
		
		return apply(fun, tree(LEFT,dim,arg));
		
	} else if (e->isa(APPLYRIGHT,&dim,&fun,&arg)) {
		
		return apply(fun, tree(RIGHT,dim,arg));
		
	} else if (e->isa(RIGHT,&dim,&exp)) {
		
		return right(dim, exp);
		
	} else if (e->isa(LEFT,&dim,&exp)) {
		
		return left(dim, exp);
		
	} else {
		
		return e;
		
	}
}


// forces a suspended computation
Tree* force(Tree* e)
{
	int		dim;
	Tree	*exp, *env, *fun, *arg, *r;
	
	while (1) {
		if (e->isa(EVAL,&exp,&env)) {

			r = eval(exp,env);

		} else if (e->isa(APPLYLEFT,&dim,&fun,&arg)) {

			r = apply(fun, tree(LEFT,dim,arg));

		} else if (e->isa(APPLYRIGHT,&dim,&fun,&arg)) {

			r = apply(fun, tree(RIGHT,dim,arg));

		} else if (e->isa(RIGHT,&dim,&exp)) {

			r = right(dim, exp);

		} else if (e->isa(LEFT,&dim,&exp)) {

			r =  left(dim, exp);

		} else {

			return e;
		}
		e = r;
	}
	
	return e; // a revoir
}

// takes the right part on the <dim> dimension of expression <e> 
Tree* right(int dim1, Tree* e)
{
	int		dim2;
	Tree	*left, *right;
	
	e = force(e);
	
	if (e->isa(CONSTRUCT, &dim2, &left, &right)) {
		if (dim1 == dim2) {
			return right;
		} else {
			return tree(CONSTRUCT, dim2, tree(RIGHT,dim1,left), tree(RIGHT,dim1,right));
		}
	} else {
		return e;
	}
}	
	

// takes the left part on the <dim> dimension of expression <e> 
Tree* left(int dim1, Tree* e)
{
	int		dim2;
	Tree	*left, *right;
	
	e = force(e);
	
	if (e->isa(CONSTRUCT, &dim2, &left, &right)) {
		if (dim1 == dim2) {
			return left;
		} else {
			return tree(CONSTRUCT, dim2, tree(LEFT,dim1,left), tree(LEFT,dim1,right));
		}
	} else {
		return e;
	}
}	
	

// recall in a debruijn environment <env> the value stored at <level>
Tree* search( Tree* env, int level)
{
	Tree *next, *val = 0;
	if (0) {
		cout << "search " << level << " in ";
		env->print(cout);
		cout << endl;
	}
	
	while ( env->isa(ENV, &val, &next) && level > 0 ) { env = next; level--; }
	assert(val);
	return val;
}
