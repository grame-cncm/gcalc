#include <iostream>

#include <QPainter>
#include <QLinearGradient>

#include "PaletteWidget.h"
#include "WorkspaceWidget.h"
#include "images/palette.xpm"
#include "Dragger.h"

#include "EvalRules.h"
#include "constructions.h"
#include "cubes.h"


//---------- operations for the drag and drop palette -----------
// x is the target term and d is the drag&drop term
//---------------------------------------------------------------

static Tree* putleft	(Tree* x, Tree* d) { return HorzExp(d,x); }
static Tree* putright	(Tree* x, Tree* d) { return HorzExp(x,d); }

static Tree* puttop		(Tree* x, Tree* d) { return VertExp(d,x); }
static Tree* putbot		(Tree* x, Tree* d) { return VertExp(x,d); }
 
static Tree* putback	(Tree* x, Tree* d) { return ProfExp(x,d); }
static Tree* putfront	(Tree* x, Tree* d) { return ProfExp(d,x); }

static Tree* putabstr	(Tree* x, Tree* d) { return AbstrExp(d,x);}
static Tree* putarg		(Tree* x, Tree* d) { return ApplExp(x,d); }
static Tree* putfun		(Tree* x, Tree* d) { return ApplExp(d,x); }


//-----------------------------------------------------------------
// palette(wb) : create a hidden palette to be placed on the 
// workbench 
//-----------------------------------------------------------------
PaletteWidget::PaletteWidget(WorkspaceWidget* workspace) :
		fPixmap(palette_xpm),
		fLines(3), 
		fCols(3),
		fZoneNum(-1)
{
	setParent(workspace);
	hide();
	resize(fPixmap.size());

	// table des operations
	fOperation[0] = &putabstr;
	fOperation[1] = &putleft;
	fOperation[2] = &putfront;
	
	fOperation[3] = &puttop;
	fOperation[4] = &putfun;
	fOperation[5] = &putbot;
	
	fOperation[6] = &putback;
	fOperation[7] = &putright;
	fOperation[8] = &putarg;

}


void PaletteWidget::showCenteredOn(QWidget* w)
{
	QRect r = geometry();
	r.moveCenter(w->geometry().center());
	setGeometry(r);
	setWindowOpacity(0.2);
	raise();
	show();
}

bool PaletteWidget::contains(const QPoint& p)
{
	return geometry().contains(p, true);
}

void PaletteWidget::highlightZone(const QPoint& p)
{
	int h,v; getZone(p,&h,&v);
	QRect r (h*width()/fCols, v*height()/fLines, width()/fCols, height()/fLines);
	if (r != fHighlightRect) {
		fHighlightRect = r;
		fZoneNum = (h<fCols & v<fLines) ? h*fCols+v : -1;
		update();
	}
}

void PaletteWidget::getZone(const QPoint& p, int* h, int* v)
{
	*h = fCols*p.x()/width();
	*v = fLines*p.y()/height();
}

Tree* PaletteWidget::compute(Tree* dst, Tree* drag)
{
	assert(fZoneNum>-1 & fZoneNum<9);
	return fOperation[fZoneNum](dst,drag);
}


void PaletteWidget::paintEvent(QPaintEvent*) 
{
// 	QColor	pale(255,255,255,100);
 	QColor	pale(240,240,240,150);
 	QColor	high(255,120,120,150);

	//
 	//QColor	pale1(120,120,120,100);
 	//QColor	pale1(120,120,120,30);
 	QColor	pale1(60,60,60,20);
 	QColor	pale2(240,240,240,150);
	QLinearGradient linearGrad(QPointF(20, 50), QPointF(220, 210));
    linearGrad.setColorAt(0, pale2);
    linearGrad.setColorAt(0.1, pale1);
    linearGrad.setColorAt(0.6, pale2);
    linearGrad.setColorAt(1, pale1);
	QBrush mybrush(linearGrad);
	//

	QPainter painter(this);
	//painter.fillRect(rect(),pale);
	painter.fillRect(rect(),mybrush);
	if (fZoneNum>=0) painter.fillRect(fHighlightRect,high);
	painter.drawPixmap(0,0,fPixmap);
}

#if 0

void PaletteWidget::mousePressEvent(QMouseEvent* event) 
{
	//gDragger.init(this,event);
	highlightZone(event->pos());
	raise();
	
	std::cout << "Palette mouse presssed" << std::endl;
}

void PaletteWidget::mouseMoveEvent(QMouseEvent* event) 
{
	//gDragger.drag(this,event);
	std::cout << "Palette mouse moved" << std::endl;
}

void PaletteWidget::mouseReleaseEvent(QMouseEvent* event) 
{
	std::cout << "Palette mouse released" << std::endl;
}

#endif
