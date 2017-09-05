#ifndef __Dragger__
#define __Dragger__

#include <QPoint>
#include <QMouseEvent>

class WorkspaceWidget;
class IconWidget;

class Dragger
{
	WorkspaceWidget*	fWorkspace;
	int					fStartZone;
	IconWidget*			fSrcIcon;
	IconWidget*			fDraggedIcon;
	QPoint				fGlobalMouseOrigin;
	QPoint				fDraggedWidgetOrigin;
	IconWidget*			fTargetIcon;
	bool				fDragging;

  public:
  
	Dragger(WorkspaceWidget* workspace);
	void init	(IconWidget* w, QMouseEvent* e);
	void drag	(QMouseEvent* e);
	void drop	(QMouseEvent* e);
	void cancel	();

};

#endif
