//---------------------------------------------------------------------
//---------------------------------------------------------------------
// This example shows how to create a window with a rayures background
// with Qt4 with another widget inside
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#include "images/rayures4.xpm"

#include "PrimitivesWidget.h"
#include "WorkspaceWidget.h"
#include "IconWidget.h"
#include "constructions.h"
#include "cubes.h"

PrimitivesWidget::PrimitivesWidget(WorkspaceWidget* workspace, int ox, int oy, int csize) 
	: PixWidget(QPixmap(rayures4_xpm), 492, 60),  
      fWorkspace(workspace)
{
	setParent(fWorkspace);
	move(ox,oy);
}


static Tree* naming(const char* name, Tree* term)
{
	setNameProperty(term, name);
	return term;
}

void PrimitivesWidget::populate()
{ 
	int ox = x();
	int oy = y();
	
	// create the primitive icons
	fWorkspace->addIcon(ox+6, oy+4, new IconWidget(naming("white", WhiteExp())));
	fWorkspace->addIcon(ox+6+1*54, oy+4, new IconWidget(naming("red", RedExp())));
	fWorkspace->addIcon(ox+6+2*54, oy+4, new IconWidget(naming("blue", BlueExp())));
	fWorkspace->addIcon(ox+6+3*54, oy+4, new IconWidget(naming("green", GreenExp())));
	fWorkspace->addIcon(ox+6+4*54, oy+4, new IconWidget(naming("black", BlackExp())));
	fWorkspace->addIcon(ox+6+5*54, oy+4, new IconWidget(naming("invisible", InvisExp())));

	fWorkspace->addIcon(ox+6+6*54, oy+4, new IconWidget(naming("horz", HorzPrimExp())));
	fWorkspace->addIcon(ox+6+7*54, oy+4, new IconWidget(naming("vert", VertPrimExp())));
	fWorkspace->addIcon(ox+6+8*54, oy+4, new IconWidget(naming("back", ProfPrimExp())));

}

