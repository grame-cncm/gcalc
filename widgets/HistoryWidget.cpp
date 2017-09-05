//---------------------------------------------------------------------
//---------------------------------------------------------------------
// This example shows how to create a window with a rayures background
// with Qt4 with another widget inside
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#include "images/rayures3.xpm"
#include "images/history.xpm"

#include "HistoryWidget.h"
#include "WorkspaceWidget.h"
#include "IconWidget.h"

#include "tlib.h"

HistoryWidget::HistoryWidget(WorkspaceWidget* workspace, int ox, int oy, int csize)
	: PixWidget(QPixmap(rayures3_xpm), 60, 700, QPixmap(history_xpm), -2, 16),  
      fWorkspace(workspace),
      fSize(csize)
{
	setParent(fWorkspace);
	move(ox,oy);
	prepare(9, 116, 64);
	show();
}

void HistoryWidget::prepare (int ox, int oy, int dy) 
{
	for (int i = 0; i < HISTSIZE; i++)
	{
		X[i] = ox;
		Y[i] = oy + i*dy;
		I[i] = 0;
	}
}

void HistoryWidget::push (Tree* term) 
{
	if (!member(term)) {
		slidedown();
		I[0] = new IconWidget(term);
		fWorkspace->addIcon(X[0], Y[0], I[0]);
	} else {
		// already in the history
		// delete it
	}
}		

// mettre en place un systeme ou l'on detruit les vielles copies dans l'historique
// de fa�on a ce que les choses souvent utilis�es soient en haut
bool HistoryWidget::member(Tree* e)
{
	for (int i=0; i<HISTSIZE && I[i]; i++)
	{
		if (I[i]->term() == e) return true;
	}
	return false;
}

void HistoryWidget::slidedown()
{
	int i = HISTSIZE-1;
	if (I[i] != 0) { 
		fWorkspace->remIcon(I[i]);
	}

	do {
		I[i] = I[i-1];
		if (I[i]) { I[i]->move(X[i],Y[i]); }
	} while (--i);
}
