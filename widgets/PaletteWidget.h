#ifndef __PaletteWidget__
#define __PaletteWidget__

#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include "tlib.h"

class WorkspaceWidget;

typedef Tree* (* CubeOp)(Tree*, Tree*);

class PaletteWidget : public QWidget
{
	QPixmap		fPixmap;
	int			fLines;
	int			fCols;
	CubeOp		fOperation[9];
	QRect		fHighlightRect;
	int			fZoneNum;

  public:

	PaletteWidget(WorkspaceWidget* workspace);
  
	void 	showCenteredOn (QWidget* w);
	bool	contains (const QPoint& p);
	void 	highlightZone (const QPoint& p);
	Tree* 	compute(Tree* x, Tree* d);

	virtual void paintEvent(QPaintEvent* event);
// 	virtual void mousePressEvent(QMouseEvent* event); 
// 	virtual void mouseMoveEvent(QMouseEvent* event); 
// 	virtual void mouseReleaseEvent(QMouseEvent* event); 


	
  private:
	void	getZone(const QPoint& p, int* h, int* v);
	void	init();
	
};
	 

#endif
