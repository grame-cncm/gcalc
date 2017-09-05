//---------------------------------------------------------------------
//---------------------------------------------------------------------
// This example shows how to create a window with a rayures background
// with Qt4 with another widget inside
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#include <QBrush>
#include "PixWidget.h"


void PixWidget::setBackgroundPixmap(const QPixmap& pixmap)
{
	QPalette 	p;
	p.setBrush(backgroundRole(), QBrush(pixmap));
	setPalette(p);
	setAutoFillBackground(true);
}

PixWidget::PixWidget(const QPixmap& pixmap)
{
	setBackgroundPixmap(pixmap);
	resize(pixmap.size());
}

PixWidget::PixWidget(const QPixmap& pixmap, int w, int h)
{
	setBackgroundPixmap(pixmap);
	resize(w, h);
}

PixWidget::PixWidget(const QPixmap& pixmap, int l, int h, const QPixmap& logo, int ox, int oy)
{
	setBackgroundPixmap(pixmap);
	resize(l, h);
	QWidget* w = new PixWidget(logo);
	w->setParent(this);
	w->move(ox,oy);
	w->show();
}
