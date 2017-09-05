//---------------------------------------------------------------------
//---------------------------------------------------------------------
// This example shows how to create a window with a rayures background
// with Qt4 with another widget inside
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#ifndef __HistoryWidget__
#define __HistoryWidget__

#include "PixWidget.h"

#define HISTSIZE 10

class WorkspaceWidget;
class IconWidget;
class Tree;

class HistoryWidget : public PixWidget
{
	WorkspaceWidget*	fWorkspace;
    int					fSize;
    int					X[HISTSIZE];
	int					Y[HISTSIZE];
	IconWidget*			I[HISTSIZE];
	
	void 	prepare (int x, int y, int dy);
	bool 	member 	(Tree* e);
	void 	slidedown ();
	
 public:

	HistoryWidget(WorkspaceWidget* workspace, int x, int y, int size=60);
 	void push (Tree* e);
};

#endif
