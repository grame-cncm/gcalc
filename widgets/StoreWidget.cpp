//---------------------------------------------------------------------
//---------------------------------------------------------------------
// This example shows how to create a window with a rayures background
// with Qt4 with another widget inside
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#include "images/rayures2.xpm"
#include "images/storage.xpm"

#include "StoreWidget.h"
#include "WorkspaceWidget.h"

StoreWidget::StoreWidget(WorkspaceWidget* workspace, int ox, int oy)
	: PixWidget(QPixmap(rayures2_xpm), 800, 800, QPixmap(storage_xpm), -2, 112),
      fWorkspace(workspace)
{
	setParent(fWorkspace);
	move(ox,oy);
	show();
}

