//---------------------------------------------------------------------
//---------------------------------------------------------------------
// This example shows how to create a window with a rayures background
// with Qt4 with another widget inside
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#ifndef __WorkspaceWidget__
#define __WorkspaceWidget__

#include <QSet>
#include <QMouseEvent>

#include "PixWidget.h"
#include "HistoryWidget.h"
#include "PrimitivesWidget.h"
#include "StoreWidget.h"
#include "IconWidget.h"
#include "PaletteWidget.h"
#include "Dragger.h"

typedef QSet<IconWidget*> IconSet;
enum { kOutside, kWorkspace, kHistory, kPrimitives, kStorage };

class WorkspaceWidget : public PixWidget
{
	Dragger				fDragger;
	HistoryWidget		fHistory;
	PrimitivesWidget	fPrimitives;
	StoreWidget			fStorage;
	PaletteWidget		fPalette;

	IconSet		fIcons;
	IconSet		fSelected;

 public:

	WorkspaceWidget();
	virtual ~WorkspaceWidget();

	// icon management
	void	addIcon(int x, int y, IconWidget* icon);
	void	addIcon(const QPoint& p, IconWidget* icon);
	void	remIcon(IconWidget* icon, bool destroy=true);
	IconWidget*	dupIcon(IconWidget* src);

	IconWidget*	findNeighbour(IconWidget* icon, IconWidget* excluded=NULL);
	
	// selection management
	void	deselectAll();
	void	deselect(IconWidget* icon);
	void	select(IconWidget* icon);
	void	deleteSelection();
	void	zoomInSelection();
	void	zoomOutSelection();
	void	exportSelectionPOV();

	// mouse management
	void	mousePressEvent(QMouseEvent* event);
	void	mouseMoveEvent(QMouseEvent* event);
	void	mouseReleaseEvent(QMouseEvent* event);
	
	// keyboard management
	void 	keyPressEvent(QKeyEvent *event);

	// palette management
	void 	paletteShow (QWidget* w);
	void 	paletteHide ();
	bool	paletteContains (const QPoint& p);
	void 	paletteHighlightZone (const QPoint& p);
	void 	paletteCompute(IconWidget* x, IconWidget* d);


	int 	findZoneID(const QPoint& p);

};

#endif
