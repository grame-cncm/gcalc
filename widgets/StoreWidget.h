//---------------------------------------------------------------------
//---------------------------------------------------------------------
// This example shows how to create a window with a rayures background
// with Qt4 with another widget inside
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#ifndef __StoreWidget__
#define __StoreWidget__

#include "PixWidget.h"
class WorkspaceWidget;

class StoreWidget : public PixWidget
{
	WorkspaceWidget*	fWorkspace;
 public:

    StoreWidget(WorkspaceWidget* workspace, int x, int y);
};

#endif
