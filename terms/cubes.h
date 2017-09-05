#ifndef __CUBES__
#define __CUBES__ 

#include "tree.h"

extern Symbol*	CUBE; 

// color values used for basic colored cubes

extern int rgbaWhite;
extern int rgbaRed  ;
extern int rgbaGreen;
extern int rgbaBlue ;
extern int rgbaBlack;

extern int rgbaGrey ;
extern int rgbaTranspGrey;

// constructor and predicat for colored cubes

Tree* 	coloredCube (int color);				///< create a basic colored cube
bool 	coloredCubep (Tree* t, int* color);		///< test if t is a basic colored cube and get its color

// basic colored cube constructors

Tree* WhiteExp();
Tree* RedExp();	
Tree* GreenExp();
Tree* BlueExp();	
Tree* BlackExp();
Tree* InvisExp();

Tree* AbortExp();
#endif
