//---------------------------------------------------------------------
//---------------------------------------------------------------------
// This example shows how to create a window with a rayures background
// with Qt4 with another widget inside
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#ifndef __PixWidget__
#define __PixWidget__

#include <QWidget>
#include <QPixmap>

class PixWidget : public QWidget
{
	void setBackgroundPixmap(const QPixmap& pixmap);

 public:

	PixWidget(const QPixmap& pixmap);
	PixWidget(const QPixmap& pixmap, int width, int height);
	PixWidget(const QPixmap& pixmap, int width, int height, const QPixmap& logo, int x, int y);
};

#endif
