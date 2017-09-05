#ifndef __IconWidget__
#define __IconWidget__


#include <iostream>
#include <fstream>
#include <QWidget>
#include <QMouseEvent>
#include <QPixmap>
#include <QMoveEvent>
#include <QResizeEvent>
#include <QLabel>
#include <QLineEdit>


using namespace std;

#define ICONSIZE 50
#define WORKSIZE 75
#define MAXWORKSIZE 512

class Tree;

class IconWidget : public QWidget
{
	Q_OBJECT 
	Tree*		fTerm;
	QSize		fStoreSize;
	QSize		fWorkSize;
	QPixmap*	fPixmap;
	bool		fSelected;
	//QLabel*		fLabel;
	QLineEdit*		fLabel;
	
  public:

	IconWidget(Tree* term, const QSize& size = QSize(ICONSIZE,ICONSIZE), const QSize& worksize = QSize(WORKSIZE,WORKSIZE));
	
	virtual ~IconWidget();

	virtual void paintEvent(QPaintEvent* event);
	
	Tree*		term() 		{ return fTerm; }
	QSize		storesize() { return fStoreSize; }
	QSize		worksize() 	{ return fWorkSize; }
	QPixmap*	pixmap() 	{ return fPixmap; }
	bool		selected() 	{ return fSelected; }

	void		setCenteredSize(const QSize& s);
	void		adaptSizeToZone();
	void		zoomIn();
	void		zoomOut();

	QPoint		center()	{ return geometry().center(); }
	int			zone();
	
	void		showSelected();
	void		showDeselected();

  public slots:
	
	void 		changeNameProperty();

  protected:

	void		moveEvent (QMoveEvent * event);
	void		resizeEvent (QResizeEvent * event);	

  private:
  
	void	checkPixmapUpToDate();
	void	adaptLabel();
	void	createLabel();

};

#endif
