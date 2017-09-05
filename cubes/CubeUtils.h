#ifndef __cubeutils__
#define __cubeutils__

#include <QPainter>
#include <QPainterPath>
#include <QColor>

//********************************************************************
//
// A 3D cube that can be draw with a painter and splitted according
// to the GCalculus rules
//
//********************************************************************

class cube 
{

 public :
	short	ox;
	short	oy;
	short	coord[3][2];

 protected :
	
	void MoveTo3D(QPainterPath& p, short x, short y, short z);
	void LineTo3D(QPainterPath& p, short x, short y, short z);
	void SetSize (short c); 
	
 public:
	cube(short size) 	{ SetSize(size); }
	cube() 				{ }
 
	void Split(short d, cube& top, cube& bot);
	void SplitHorz(cube& left, cube& right) { Split(0, left, right); }
	void SplitVert(cube& top, cube& bot) 	{ Split(1, top, bot); }
	void SplitProf(cube& fore, cube& back) 	{ Split(2, fore, back); }
	void SplitAbstr(cube& var, cube& body);
	void SplitAppl(cube& fun, cube& arg);
	
	void Render(QPainter& dst, const QColor& color);
	void Render(QPainter& dst, int rgba);
};



#endif

