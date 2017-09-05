#include "debruijn.h"
#include "env.h"
#include <assert.h>

using namespace std;

// de Bruijn abstractions
Symbol*  IVAR	 = symbol("ivar");
Symbol*  ILAMBDA = symbol("ilambda");


Tree* ivar(int n)
{
	return tree(IVAR, tree(n));
}


Tree* ilambda( Tree* body)
{
	return tree(ILAMBDA, body);
}


bool	ivarp(Tree* t, int* n)
{
	Tree*	u;
	if (isTree(t, IVAR, &u) && u->isInt()) {
		*n = u->getInt();
		return true;
	} else {
		return false;
	}
}


bool	ilambdap(Tree* t, Tree** body)
{
	return isTree(t, ILAMBDA, body);
}


/**
 * Create an ilambda by making variable <part> in <body>
 * Create an ilambda in de bruijn notation by replacing <part> in <body> by 
 * a <ivar>. An <abstrProp> property is added to the resulting ilambda
 * in order to remember the variable part.
 * 
 * WARNING : the current implementation is not efficient and doesn't use memoization
 **/

static Tree* abstractAtLevel(Tree* part, int l, Tree* exp);

Tree* abstract(Tree* part, Tree* exp)
{
	Tree* result = ilambda(abstractAtLevel(part, 0, exp));
	if (!getLambdaProperty(result)) {
		setLambdaProperty(result, part);
	}
	return result;
}


static Tree* abstractAtLevel(Tree* part, int l, Tree* exp)
{
	Tree*	body;
	int		n;
	
	if (part == exp) {
		return ivar(l);
		
	} else if (exp->isa(IVAR, &n)) {
		assert(n != l);		
		return exp;
		
	} else if (ilambdap(exp, &body)) {
		Tree* res = ilambda(abstractAtLevel(part, l+1, body));
		setLambdaProperty(res,getLambdaProperty(exp));		
		return res;
		
	} else {
		int a = exp->arity();
		Tree*	br[256];
	
		for (int i=0;i<a;i++) br[i] = abstractAtLevel(part, l, (*exp)[i]);
		return Tree::make(*exp,a,br);
	}
}



//-----------------------------------------------------------------------------------------
// Lambda Abstractions in de Bruijn notation need to have a property containing the 
// original value of the variable. This allows to store abstractions in de bruijn notation
// (and benefit of hash consing) while allowing to display abstractions in normal notation
//-----------------------------------------------------------------------------------------

static Tree* lambdaPK = tree(symbol("LPK"));

void	setLambdaProperty(Tree* t, Tree* v)
{
	t->setProperty(lambdaPK,v);
}

Tree*	getLambdaProperty(Tree* t)
{
	return t->getProperty(lambdaPK);
}
