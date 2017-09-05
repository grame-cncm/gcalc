#include "EvalRules.h"
#include "tlib.h"
#include "constructions.h"
#include "TFile.h"
#include "POVUtils.h"
#include "evaluate.h"
#include <time.h>
#include <iostream>


/**
 * this class is used to limit the evaluation of an expressions. It provides
 * an upper limit for the number of applications and for the number of "cuts"
 * in all the dimensions of the space.
 *
 */

class limiter 
{
	unsigned char cuts[4];		///< amount of "cuts" available
	
 public:
 	// constructors
 	limiter (const limiter& l) 
	{
 		for (int i = 0; i<4; i++) cuts[i] = l.cuts[i];
 	}
	
 	limiter ( unsigned char x) 
	{
 		for (int i = 0; i<4; i++) cuts[i] = x;
 	}
	
 	limiter& Cut(int dim) 
	{ 
 		cuts[dim] -= 1; 
		return *this;
 	}
	
 	limiter& Dup(int dim)  
	{ 
 		cuts[dim] += 1; 
		return *this;
 	}
	
	// predicats
 	bool	IsSmall() {
 		return (cuts[0]<1) | (cuts[1]<1) | (cuts[2]<1) | (cuts[3]<1);
 	}  
};


static unsigned short BinLog (unsigned short n) 
{
	unsigned short r;
	
	if (n==0) {
		r = 0;
	} else {
		r = 16;
		while (n <= 0x7FFF)  {
			n = n+n;
			r--;
		}
	}
	return r;
}


//********************************************************************
//
// LES VARIABLES GLOBALES
//
//********************************************************************

unsigned long 	gExpSize;

Tree*			gLeftOp[3];
Tree*			gRightOp[3];

Tree*			gAborted;					// une valeur qui sert pour les erreurs et les interuptions de calcul

Tree*			gHorzPrim;
Tree*			gVertPrim;
Tree*			gProfPrim;

bool			gAbortFlag;
long			gBackCount;


long UsedElements()	{ return 0; }


void InitExp()
{

}



Tree* LeftOpExp(int i);
Tree* RightOpExp(int i);

Tree* ApplExp(Tree* e1, Tree* e2) 		{ return application(e1,e2); }
Tree* AbstrExp(Tree* part, Tree* body) 	{ return abstract(part,body); }




void drawPOVExp(Tree* e, TFWriter* dst, cube& c, limiter lim)	
{ 
	// small expressions are not draw
	if (! lim.IsSmall() ) {
			
		int		rgba;
		int 	dim;
		Tree 	*left, *right, *fun, *arg, *abstr, *env;

		e = force(e);

		if (e->isa(CUBE,&rgba)) {

			POVCube(dst, 2, c, rgba);

			
		} else  if (e->isa(CONSTRUCT, &dim, &left, &right)) {

			cube l, r; c.Split(dim, l, r);
			
			lim.Cut(dim);
			
			switch (dim) {
				case 0 : 	drawPOVExp(left, dst, l, lim);  
							drawPOVExp(right, dst, r, lim); 
							break;

				case 1 : 	drawPOVExp(right, dst, r, lim);  
							drawPOVExp(left, dst, l, lim); 
							break;

				case 2 : 	drawPOVExp(right, dst, r, lim);  
							drawPOVExp(left, dst, l, lim); 
							break;

			}

		} else if (e->isa(APPL, &fun, &arg)) {

			lim.Cut(3);
			
			cube f, a; c.SplitAppl(f, a); 
			
			drawPOVExp(arg, dst, a, lim); 
			drawPOVExp(fun, dst, f, lim);

		} else if (e->isa(CLOSURE, &abstr, &env)) {

			cube v, b; c.SplitAbstr(v, b); 

			Tree* var = eval(getLambdaProperty(abstr), tree(NULLENV));	// eval the abstracted part
			drawPOVExp(apply(e,var), dst, b, lim);	// simulated an application to draw the body
			drawPOVExp(var, dst, v, lim); 			// then draw the variable part

		} else {

			std::cout << "Error in generating pov file, unknow expression : " << *e << std::endl;
		}
	}	
}
	 


void drawExp(Tree* e, QPainter& dst, cube& c, limiter lim)	
{ 
	//cout << "DRAW : "; e->print(cout); cout << endl;		
	// small expressions are not draw
	if (! lim.IsSmall() ) {
			
		int		rgba;
		int 	dim;
		Tree 	*left, *right, *fun, *arg, *abstr, *env;

		e = force(e);
		//cout << "DRAW - FORCED : "; e->print(cout); cout << endl;		

		if (e->isa(CUBE,&rgba)) {

			c.Render(dst, rgba);

			
		} else  if (e->isa(CONSTRUCT, &dim, &left, &right)) {

			cube l, r; c.Split(dim, l, r);
			
			lim.Cut(dim);
			
			switch (dim) {
				case 0 : 	drawExp(left, dst, l, lim);  
							drawExp(right, dst, r, lim); 
							break;

				case 1 : 	drawExp(right, dst, r, lim);  
							drawExp(left, dst, l, lim); 
							break;

				case 2 : 	drawExp(right, dst, r, lim);  
							drawExp(left, dst, l, lim); 
							break;

			}

		} else if (e->isa(APPL, &fun, &arg)) {

			lim.Cut(3);
			
			cube f, a; c.SplitAppl(f, a); 
			
			drawExp(arg, dst, a, lim); 
			drawExp(fun, dst, f, lim);

		} else if (e->isa(CLOSURE, &abstr, &env)) {

			cube v, b; c.SplitAbstr(v, b); 

			Tree* var = eval(getLambdaProperty(abstr), tree(NULLENV));	// eval the abstracted part
#if 0
			cout << "Draw a closure -> ";
			cout << "variable part : "; var->print(cout);
			cout << "; abstraction part : "; e->print(cout);
			cout << endl;
#endif
			drawExp(apply(e,var), dst, b, lim);	// simulated an application to draw the body
			drawExp(var, dst, v, lim); 			// then draw the variable part

		} else {

			std::cout << "Error in drawing, unknow expression : " << *e << std::endl;
		}
	}	
}



void Represent(QPainter& dst, Tree* e, int size, int evalFlag)
{
	cube	c(size*2/3); 
	limiter l( BinLog(size*4/3) ); 
	
	// dessin de e
	clock_t T1 = clock();
	if (evalFlag) { drawExp(eval(e,tree(NULLENV)), dst, c, l); } else { drawExp(e, dst, c, l); }
	clock_t T2 = clock();
	std::cout << "\nElapsed time : " << (T2-T1)*1000/CLOCKS_PER_SEC << "ms" << std::endl;
	printstat();
}


void WritePOV(Tree* e, const QString& fname)
{
	cube 		c(POVCUBESIZE);
	limiter 	l(POVLIMIT); 
	TFWriter 	f(fname);
	
	POVFileHeader(&f);
	// dessin de e
	drawPOVExp(eval(e,tree(NULLENV)), &f, c, l);
	POVFileTrailer(&f);
}


//-----------------------------------------------------------------------------------------
// Lambda Abstractions in de Bruijn notation need to have a property containing the 
// original value of the variable. This allows to store abstractions in de bruijn notation
// (and benefit of hash consing) while allowing to display abstractions in normal notation
//-----------------------------------------------------------------------------------------

static Tree* nameProperty = tree(symbol("nameProperty"));

void setNameProperty(Tree* t, const char* name)
{
	Tree* n = tree(symbol(name));
	t->setProperty(nameProperty,n);
}

const char* getNameProperty (Tree* t)
{
	Tree* n = t->getProperty(nameProperty);
	if (n==NULL) return "";
	else return name(n->getSymbol());
}






















