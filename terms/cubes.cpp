#include "cubes.h"

// colors for the basic cube elements

int		rgbaWhite 		= 0xFFFFFFFF;
int		rgbaRed 		= 0xFF0000FF;
int		rgbaGreen 		= 0x00FF00FF;
int		rgbaBlue 		= 0x0000FFFF;
int		rgbaBlack 		= 0x000000FF;
int		rgbaInvis 		= 0x00000000;

int		rgbaGrey		= 0x777777FF;
int		rgbaTranspGrey 	= 0x77777777;



// cube constructor

Symbol*	CUBE = symbol("cube");

Tree* coloredCube(int color)
{
	return tree(CUBE, tree(color));
}

// cube predicat

bool coloredCubep(Tree* t, int* color)
{
	Tree* c;
	if (isTree(t, CUBE, &c)) {
		*color = c->getInt();
		return true;
	} else {
		return false;
	}
}

// basic colored cubes

Tree* WhiteExp()	{ return coloredCube(rgbaWhite); 	}
Tree* RedExp()		{ return coloredCube(rgbaRed);  	}
Tree* GreenExp()	{ return coloredCube(rgbaGreen);  	}
Tree* BlueExp()		{ return coloredCube(rgbaBlue); 	}
Tree* BlackExp()	{ return coloredCube(rgbaBlack);  	}
Tree* InvisExp()	{ return coloredCube(rgbaInvis);  	}

Tree* AbortExp()	{ return coloredCube(rgbaTranspGrey);  	}
