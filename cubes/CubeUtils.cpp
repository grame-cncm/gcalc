//********************************************************************
//
// Implementation of a 3D cube that can be  can be draw with 
// a painter and splitted according to the GCalculus rules
//
//********************************************************************

#include "CubeUtils.h"


// ratio for darker colors used for shadows
#define kNormalLight 	100
#define kShadow1 		125
#define kShadow2		170

inline short min(short x, short y) { return (x<y) ? x : y; }
inline short p128(short a, short b, short x) { return a + (((b - a) * x) >> 7); }


void cube::SetSize (short c)
{
	ox = 0;
	oy = c/2;
	coord[0][0] = 0; coord[0][1] = c;
	coord[1][0] = 0; coord[1][1] = c;
	coord[2][0] = 0; coord[2][1] = c;
}

void cube::Split(short d, cube& u, cube& v)
{
	{ // update offsets
		u.ox = v.ox = ox;
		u.oy = v.oy = oy;
	}
	
	for (short i = 0; i < 3; i++) {
		u.coord[i][0] = v.coord[i][0] = coord[i][0];
		u.coord[i][1] = v.coord[i][1] = coord[i][1];
	}
	u.coord[d][1] = v.coord[d][0] = (coord[d][0] + coord[d][1]) >> 1;
}
	
	
void cube::SplitAppl(cube& fun, cube& arg)
{
	{ // update offsets
		fun.ox = arg.ox = ox;
		fun.oy = arg.oy = oy;
	}
	for (short i = 0; i < 3; i++) {
		fun.coord[i][0] = arg.coord[i][0] = coord[i][0];
		fun.coord[i][1] = arg.coord[i][1] = coord[i][1];
	}
	fun.coord[0][1] = p128(coord[0][0], coord[0][1], 80);
	arg.coord[0][0] = p128(coord[0][0], coord[0][1], 48);
	fun.coord[1][0] = p128(coord[1][0], coord[1][1], 48);
	arg.coord[1][1] = p128(coord[1][0], coord[1][1], 80);
	fun.coord[2][1] = arg.coord[2][0] = (coord[2][0] + coord[2][1]) >> 1;
}

void cube::SplitAbstr(cube& var, cube& body)
{
	{ // udate offsets
		var.ox = body.ox = ox;
		var.oy = body.oy = oy;
	}
	for (short i = 0; i < 3; i++) {
		var.coord[i][0] = body.coord[i][0] = coord[i][0];
		var.coord[i][1] = body.coord[i][1] = coord[i][1];
	}
	var.coord[0][1] = (coord[0][0] + coord[0][1]) >> 1;
	var.coord[1][1] = (coord[1][0] + coord[1][1]) >> 1;
	var.coord[2][1] = p128(coord[2][0], coord[2][1], 8);
	body.coord[2][0] = p128(coord[2][0], coord[2][1], 16);
}



//=================================================================================
// Cube 3D rendering
//=================================================================================

void cube::Render(QPainter& dst, int rgba)
{
	Render(dst, QColor(0xFF & (rgba>>24),0xFF & (rgba>>16),0xFF & (rgba>>8),0xFF & rgba)); 
}
void cube::Render(QPainter& dst, const QColor& color)
{	

	QColor p1 = color;
	QColor p2 = color.dark(kShadow1);
	QColor p3 = color.dark(kShadow2);
	
	{	// draw right face
		QPainterPath rightface;
		
		MoveTo3D(rightface,1,1,0); 
		LineTo3D(rightface,1,1,1); LineTo3D(rightface,1,0,1); LineTo3D(rightface,1,0,0); LineTo3D(rightface,1,1,0);
		dst.fillPath(rightface, p3);
	}
		
	{	// draw top face
		QPainterPath topface;
		
		MoveTo3D(topface, 0,0,0); 
		LineTo3D(topface, 1,0,0); LineTo3D(topface, 1,0,1); LineTo3D(topface, 0,0,1); LineTo3D(topface, 0,0,0);
		dst.fillPath(topface,p2);
	}
	
	{	// draw front face
		QPainterPath frontface;
		
		MoveTo3D(frontface,0,1,0); 
		LineTo3D(frontface,1,1,0); LineTo3D(frontface,1,0,0); LineTo3D(frontface,0,0,0); LineTo3D(frontface,0,1,0);
		dst.fillPath(frontface,p1);
	}

}


void cube::MoveTo3D(QPainterPath& p, short x, short y, short z)
{
	short m = coord[2][z]>>1;
	p.moveTo(ox + coord[0][x] + m, oy + coord[1][y] - m);
}

void cube::LineTo3D(QPainterPath& p, short x, short y, short z)
{
	short m = coord[2][z]>>1;
	p.lineTo(ox + coord[0][x] + m, oy + coord[1][y] - m);
}

