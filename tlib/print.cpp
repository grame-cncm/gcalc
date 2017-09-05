#include "print.h"
#include "env.h"
#include "debruijn.h"

static void printWithEnv (Tree* t, ostream& fout, Env* env)
{
	Tree*	body = 0;
	int		level;
	
	assert(t);
	
	if (ilambdap(t, &body)) {
		
		Tree* part = getLambdaProperty(t);
		assert(part);
		fout << "lambda[";
		printWithEnv(part,fout,NULL);
		fout << ',';
		{ 
			Env E(part,env); 
			printWithEnv(body,fout,&E); 
		}
		fout << ']';
		
	} else if (ivarp(t,&level)) {	
			
		printWithEnv(env->retrieve(level),fout,NULL);
		
	} else {
		t->Node::print(fout);
		int a = t->arity();
		if (a > 0) {
			int i; char sep;
			for (sep = '[', i = 0; i < a; sep = ',', i++) {
				fout << sep; printWithEnv((*t)[i],fout,env); 
			}
			fout << ']';
		} 
	}
}


void print (Tree* t, ostream& fout)
{
	printWithEnv(t,fout,NULL);
}

