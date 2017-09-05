//---------------------------------------------------------------------
//---------------------------------------------------------------------
// This example shows how to create a window with a rayures background
// with Qt4 with another widget inside
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#ifndef __PrimitivesWidget__
#define __PrimitivesWidget__

#include "PixWidget.h"

class WorkspaceWidget;

class PrimitivesWidget : public PixWidget
{
	WorkspaceWidget*	fWorkspace;
	
 public:

	PrimitivesWidget(WorkspaceWidget* workspace, int x, int y, int size=50);
	void populate();
};

#endif
