#ifndef __POVUTILS__
#define __POVUTILS__

#include "TFile.h"
#include "CubeUtils.h"


#define POVCUBESIZE 2048
#define POVLIMIT	10
#define POVCUBECOEF	512.0


void POVFileHeader (TFWriter* f);
void POVFileTrailer (TFWriter* f);
void POVUnionHeader (TFWriter* f, int tab);
void POVUnionTrailer (TFWriter* f, int tab, cube& c);
void POVCube (TFWriter* f, int tab, cube& c, int rgba);
void POVBox (TFWriter* f, cube& c);
void POVTexture (TFWriter* f, int rgba);
void POVTab (TFWriter* f, int tab);

#endif
