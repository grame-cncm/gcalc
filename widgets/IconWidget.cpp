#include <QPainter>
#include <QSize>
#include "IconWidget.h"
#include "CubeUtils.h"
#include "Dragger.h"
#include "EvalRules.h"
#include "WorkspaceWidget.h"
#include "constructions.h"
#include <iostream>


IconWidget::IconWidget(Tree* exp, const QSize& ssize, const QSize& wsize) 
	: fTerm(exp), fStoreSize(ssize), fWorkSize(wsize), fPixmap(0), fSelected(false)
{
	resize(ssize);
	fLabel = NULL;
	const char* name = getNameProperty(fTerm);
	if (name[0]!=0) setToolTip(name);
}

IconWidget::~IconWidget() 
{
	if (fLabel) delete fLabel;
}

void IconWidget::checkPixmapUpToDate() 
{
	if (fPixmap==0 || fPixmap->size() != size()) {
		if (fPixmap != 0) { delete fPixmap; }
		fPixmap = new QPixmap(size());
		fPixmap->fill(QColor(0,0,0,0));
		QPainter painter(fPixmap);
		painter.setPen(palette().windowText().color());
		painter.drawRect(0,0,width()-1, height()-1);
		Represent(painter, fTerm, min(width(), height()), true);
	}

}

void IconWidget::showSelected()
{
	if (!fSelected) {
		fSelected = true;
		update();
	}
}


void IconWidget::showDeselected()
{
	if (fSelected) {
		fSelected = false;
		update();
	}
}


void IconWidget::paintEvent(QPaintEvent*) 
{
	checkPixmapUpToDate	();
	QPainter painter(this);
	painter.drawPixmap(0,0,*fPixmap);
	if (fSelected) { 
		painter.setPen(palette().dark().color()); 
	} else { 
		painter.setPen(palette().light().color()); 
	}  
	painter.drawRect(0,0,width()-1, height()-1);
}


int IconWidget::zone()
{
	WorkspaceWidget* w = dynamic_cast<WorkspaceWidget*>(parent());
	return (w) ? w->findZoneID(this->center()) : kOutside;
}


void IconWidget::setCenteredSize(const QSize& s)
{
	if (s != size()) {
		QRect r = QRect(pos(), s);
		r.moveCenter(center());
		setGeometry(r);
	}
}

void IconWidget::adaptSizeToZone()
{
	switch (zone()) 
	{
	  case kWorkspace :
		if (fLabel) fLabel->hide();
		setCenteredSize(fWorkSize);
		break;

	  case kStorage :
		setCenteredSize(fStoreSize);
		if (fLabel==NULL) createLabel();
		fLabel->show();
		break;

	  default :
		if (fLabel) fLabel->hide();
		setCenteredSize(fStoreSize);
	}
}

void IconWidget::zoomIn()	
{
	//std::cout << "zoom in" << std::endl;
    if (fWorkSize.width() < MAXWORKSIZE) fWorkSize *= 1.20;
	adaptSizeToZone();
}

void IconWidget::zoomOut()	
{
	//std::cout << "zoom out" << std::endl;
    if (fWorkSize.width() > WORKSIZE) fWorkSize /= 1.20;
	adaptSizeToZone();
}

void IconWidget::moveEvent (QMoveEvent *) 
{
	//std::cout << "move event" << std::endl;
	adaptLabel();
}

void IconWidget::resizeEvent (QResizeEvent *) 
{
	//std::cout << "resize event" << std::endl;
	adaptLabel();
}	

void IconWidget::adaptLabel()
{
	if (fLabel) {
		QRect r(geometry());
		int oy = r.bottom()+2;
		int	ox = (r.left()+r.right()-fLabel->width())/2 ;
		fLabel->move(ox, oy);
	}
}

void IconWidget::createLabel()
{
	assert(fTerm);
	const char* name = getNameProperty(fTerm);
	fLabel = new QLineEdit(name, parentWidget()); 
	fLabel->setAlignment(Qt::AlignHCenter);
	fLabel->resize(WORKSIZE,20);
	fLabel->show();
	connect(fLabel,SIGNAL(editingFinished()), this, SLOT(changeNameProperty()));
}

void IconWidget::changeNameProperty()
{
	assert(fLabel);
	//std::cout << "changeNameProperty : " << fLabel->text().toLatin1().constData() << std::endl;
	assert(fTerm);
	setToolTip(fLabel->text());
	setNameProperty(fTerm,  fLabel->text().toLatin1().constData());
}


