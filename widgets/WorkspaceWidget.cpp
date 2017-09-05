//---------------------------------------------------------------------
//---------------------------------------------------------------------
// This example shows how to create a window with a rayures background
// with Qt4 with another widget inside
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#include <QApplication>
#include <QPoint>
#include <QRect>

#include <QString>
#include <QWidget>
#include <QPixmap>
#include <QFileDialog>
#include <assert.h>

#include "WorkspaceWidget.h"
#include "tlib.h"
#include "Dragger.h"
#include "EvalRules.h"

#include "images/rayures1.xpm"


static const char* zonename(int id)
{
	const char* N[]={ "kOutside", "kWorkspace", "kHistory", "kPrimitives", "kStorage" };
	return N[id];
}

WorkspaceWidget::WorkspaceWidget() 
	: PixWidget(QPixmap(rayures1_xpm), 1000, 800), 
	  fDragger (this),
	  fHistory (this, 4, 100, 50),
	  fPrimitives (this, 4, 4, 50),
	  fStorage (this, 700, 4),
	  fPalette(this)
{
	fPrimitives.populate();
	setFocusPolicy(Qt::StrongFocus);
}

WorkspaceWidget::~WorkspaceWidget()
{
	//delete fHistory;
	//delete fPrimitives;
	//delete fStorage;
	
	foreach(IconWidget* icon,fIcons) delete icon;
}


void WorkspaceWidget::deleteSelection()
{
	foreach(IconWidget* icon,fSelected) {
		int z = icon->zone();
		if (z == kWorkspace || z == kStorage) {
			icon->showDeselected();
			remIcon(icon);
		}
	}
}


void WorkspaceWidget::zoomInSelection()
{
	foreach(IconWidget* icon,fSelected) icon->zoomIn();
}


void WorkspaceWidget::zoomOutSelection()
{
	foreach(IconWidget* icon,fSelected) icon->zoomOut();
}

int filenum = 0;

void WorkspaceWidget::exportSelectionPOV()
{
	foreach(IconWidget* icon,fSelected) {
		QString fname = QFileDialog::getSaveFileName(this, "export cube as POV file");
		WritePOV(icon->term(), fname);
	}
}


void WorkspaceWidget::deselectAll()
{
	foreach(IconWidget* icon,fSelected) icon->showDeselected();
	fSelected.clear();
	fPalette.hide();
}

void WorkspaceWidget::deselect(IconWidget* icon)
{
	fSelected -= icon;
	icon->showDeselected();
	//fPalette.hide();
}

void WorkspaceWidget::select(IconWidget* icon)
{
	fSelected += icon;
	icon->showSelected();
	//fPalette.showCenteredOn(icon);
	
}

void WorkspaceWidget::addIcon(int ox, int oy, IconWidget* icon)
{
	assert(icon);
	fIcons += icon;
	icon->setParent(this);
	icon->move(ox,oy);
	icon->adaptSizeToZone();
	icon->show();
}

void WorkspaceWidget::addIcon(const QPoint& p, IconWidget* icon)
{
	assert(icon);
	fIcons += icon;
	icon->setParent(this);
	icon->move(p);
	icon->adaptSizeToZone();
	icon->show();
}

// void WorkspaceWidget::dupIcon(IconWidget* src)
// {
// 	assert(src);
// 	IconWidget*	icon = new IconWidget(src->term(), src->storesize(), src->worksize());
// 	fIcons += icon;
// 	icon->setParent(this);
// 	icon->resize(src->size());
// 	icon->move(src->pos());
// 	icon->stackUnder(src);
// 	icon->show();
// }

IconWidget* WorkspaceWidget::dupIcon(IconWidget* src)
{
	assert(src);
	IconWidget*	icon = new IconWidget(src->term(), src->storesize(), src->worksize());
	fIcons += icon;
	icon->setParent(this);
	icon->resize(src->size());
	icon->move(src->pos());
	//icon->stackUnder(src);
	icon->show();
	return icon;
}


void WorkspaceWidget::remIcon(IconWidget* icon, bool deleteFlag)
{
	assert(icon);
	fIcons -= icon;
	fSelected -= icon;
	if (deleteFlag) delete icon;
}

IconWidget*	WorkspaceWidget::findNeighbour(IconWidget* icon, IconWidget* excluded)
{
	const int delta = 8;
	// compute sensibility rectangle
	QRect r = icon->geometry();
	r.setLeft(r.left()- delta);
	r.setTop(r.top()- delta);
	r.setRight(r.right()+ delta);
	r.setBottom(r.bottom()+ delta);
	
	foreach(IconWidget* n, fIcons) {
		if (   n != icon 
			&& n != excluded 
			&& n->geometry().intersects(r) 
			&& findZoneID(n->geometry().center()) == kWorkspace)
		{
			return n;
		}
	}
	return NULL;
}


void WorkspaceWidget::mousePressEvent(QMouseEvent* e) 
{
	IconWidget* i = dynamic_cast<IconWidget*>(childAt(e->pos()));

	//if (dynamic_cast<IconWidget*>(i)) 
	//gDragger.init(this,event);
	//raise();
	std::cout << "WorkspaceWidget mouse presssed in zone " << zonename(findZoneID(e->pos())) << std::endl;
	if (i) {
		std::cout << "WorkspaceWidget mouse presssed in icon " << i << std::endl;
		i->raise();
		fDragger.init(i,e);
	} else {
		deselectAll();
	}
}

void WorkspaceWidget::mouseMoveEvent(QMouseEvent* e) 
{
/*	IconWidget* i = dynamic_cast<IconWidget*>(childAt(event->pos()));
	//gDragger.drag(this,event);
	std::cout << "WorkspaceWidget mouse moved" << std::endl;
	if (i) {
		std::cout << "WorkspaceWidget mouse moved with icon " << i << std::endl;
		fDragger.drag(event);
	}*/
	fDragger.drag(e);
}

void WorkspaceWidget::mouseReleaseEvent(QMouseEvent* e) 
{
	std::cout << "WorkspaceWidget mouse released" << std::endl;
	fDragger.drop(e);
}

int WorkspaceWidget::findZoneID(const QPoint& p)
{
	if (!rect().contains(p)) return kOutside;
	if (fHistory.geometry().contains(p))	return kHistory;
	if (fPrimitives.geometry().contains(p)) return kPrimitives;
	if (fStorage.geometry().contains(p))	return kStorage;
	return kWorkspace;
}

	
// palette management
void 	WorkspaceWidget::paletteShow (QWidget* w)
{ 
	fPalette.showCenteredOn(w);
}

void 	WorkspaceWidget::paletteHide ()
{ 
	fPalette.hide();
}


bool	WorkspaceWidget::paletteContains (const QPoint& p)
{ 
	return fPalette.contains(p);
}


void 	WorkspaceWidget::paletteHighlightZone (const QPoint& p)
{ 
	fPalette.highlightZone(fPalette.mapFromParent(p));
}


void WorkspaceWidget::paletteCompute(IconWidget* dst, IconWidget* drag)
{ 
	Tree* r = fPalette.compute(dst->term(),drag->term());
	fPalette.hide();
	fHistory.push(r);
	IconWidget* icon = new IconWidget(r);
	addIcon(dst->pos(), icon);
	deselectAll();
	select(icon);
	
	//remIcon(x);
	//remIcon(d);
}

void WorkspaceWidget::keyPressEvent(QKeyEvent *e)
{
	switch (e->key()) {
		case Qt::Key_Backspace :	
		case Qt::Key_Delete :	
			deleteSelection();
			break;
		case Qt::Key_Up :	
			zoomInSelection();
			break;
		case Qt::Key_Down :	
			zoomOutSelection();
			break;
		case Qt::Key_X :	
			exportSelectionPOV();
			break;
		default : 
			QWidget::keyPressEvent(e);
			break;
	}
}



