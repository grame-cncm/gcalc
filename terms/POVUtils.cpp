#include "POVUtils.h"

#define kInclude 2000
#define kCamera 2001
#define kLight 2002
#define kSky 2003
#define kFloor 2004

void POVFileHeader (TFWriter* f)
{
	f->Puts("global_settings { assumed_gamma 1.5 noise_generator 2 }\n\n");
	f->Puts( "union { \n" );
}


void POVFileTrailer (TFWriter* f)
{
	//f->Puts( "bounded_by { " ); POVBox (f, c); f->Puts( " }\n" );
	f->Puts( "translate <-2, -2, -2>\n" );
	f->Puts( "rotate 360*y*clock\n" );
	f->Puts( "}\n\n");
	
	f->Puts( " light_source { <6, 5, -5>, rgb <1, 1, 1> }\n");
	f->Puts( " light_source { <4, 5, -5>, rgb <1, 1, 1> }\n\n");

	f->Puts( 
" camera {\n\
   perspective\n\
   location <7.0, 4, -6>\n\
   sky <0, 1, 0>\n\
   direction <0, 0, 1>\n\
   right <1, 0, 0>\n\
   up <0, 1, 0>\n\
   look_at <0, 0, 0>\n\
 }\n\n");
 
}

#if 0
void POVUnionHeader (TFWriter* f, int tab)
{
	POVTab(f, tab); f->Puts( "union { \n" );
}

void POVUnionTrailer (TFWriter* f, int tab, cube& c)
{
	POVTab(f, tab+1); f->Puts( "bounded_by { " ); POVBox (f, c); f->Puts( " }\n" );
	POVTab(f, tab+1); f->Puts( "translate <-2, -2, -2>\n" );
	POVTab(f, tab+1); f->Puts( "rotate 360*y*clock\n" );
	POVTab(f, tab); f->Puts( "}\n");
}

#else

void POVUnionHeader (TFWriter*, int )
{
}

void POVUnionTrailer (TFWriter* , int , cube& )
{
}
#endif

void POVCube (TFWriter* f, int tab, cube& c, int rgba)
{
	if ((rgba & 0xFF) != 0) {
		// only visible cubes are generated
		POVTab(f, tab); f->Puts( "object {\n" );
		POVTab(f, tab+1); POVBox (f, c); 
		POVTab(f, tab+1); POVTexture (f, rgba); f->Putc('\n');
		POVTab(f, tab); f->Puts( "}\n\n" );
	}
}

void POVBox (TFWriter* f, cube& c)
{
	// ATTENTION inversion de l'axe des y par rapport au Mac.
	// Ceci est fait pour une dimension fixe de cube : 4.0
	f->Puts( "box { <");
	f->Putf(double(c.coord[0][0])/POVCUBECOEF); f->Putc(' ');
	f->Putf(4.0 - double(c.coord[1][0])/POVCUBECOEF); f->Putc(' ');
	f->Putf(double(c.coord[2][0])/POVCUBECOEF); 
	f->Puts("> <");
	f->Putf(double(c.coord[0][1])/POVCUBECOEF); f->Putc(' ');
	f->Putf(4.0 - double(c.coord[1][1])/POVCUBECOEF); f->Putc(' ');
	f->Putf(double(c.coord[2][1])/POVCUBECOEF), 
	f->Puts("> }\n");
}

void POVTexture (TFWriter* f, int rgba)
{
	f->Puts( "pigment { color rgb <"); 
	f->Putc(' '); f->Putf(((rgba >> 24)&0xFF)/256.0);
	f->Putc(' '); f->Putf(((rgba >> 16)&0xFF)/256.0);
	f->Putc(' '); f->Putf(((rgba >> 8)&0xFF)/256.0);
	f->Puts( "> }");
}

void POVTab (TFWriter* f, int tab)
{
	if (tab > 32) tab = 32;
	char s[33];
	char* p = s;
	while (tab--) *p++ = ' ';
	*p++ = 0;
	
	f->Puts(s);
}

