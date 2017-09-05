#include "Dragger.h"
#include "IconWidget.h"
#include "WorkspaceWidget.h"
#include <iostream>


Dragger::Dragger(WorkspaceWidget* workspace) 
	: fWorkspace(workspace),
	  fDraggedIcon(NULL)
{

}

int gCount = 0;
const int  delta = 6;

void Dragger::init(IconWidget* icon, QMouseEvent* e)
{
	assert(icon);
	fWorkspace->deselectAll();
	fWorkspace->select(icon);
	fSrcIcon = NULL;
	fDraggedIcon = icon;
	fStartZone = icon->zone();
	fGlobalMouseOrigin = e->globalPos();
	fDraggedWidgetOrigin = icon->pos();
	fDragging = false;
}

void Dragger::drag(QMouseEvent* e)
{
	if (fDragging) {
		//std::cout << "drag " << gCount++ << std::endl;
		fDraggedIcon->move(fDraggedWidgetOrigin + (e->globalPos() - fGlobalMouseOrigin));
		fDraggedIcon->adaptSizeToZone();
		if (fTargetIcon) {
			if (fWorkspace->paletteContains(e->pos())) {
				fWorkspace->paletteHighlightZone(e->pos());
			} else {
				fWorkspace->paletteHide();
				fTargetIcon = NULL;
			}
		} else if (fDraggedIcon->zone() == kWorkspace) {
			fTargetIcon = fWorkspace->findNeighbour(fDraggedIcon);
			if (fTargetIcon) {
				// we have found a target
				//fDraggedIcon->hide();
				fWorkspace->paletteShow(fTargetIcon);
				fWorkspace->paletteHighlightZone(e->pos());
				fDraggedIcon->raise();
				//fDraggedIcon->show();
			}
		}
	} else if (fDraggedIcon) {
		// we need to move a minimun to start a drag
		if ((e->globalPos() - fGlobalMouseOrigin).manhattanLength() > delta) {
			fDragging = true;
			if (fStartZone == kWorkspace) {
				if (e->modifiers().testFlag(Qt::ControlModifier)) {
					// use ctrl to duplicate an icon on the workspace
					fSrcIcon = fDraggedIcon;
					fDraggedIcon = fWorkspace->dupIcon(fDraggedIcon);
					fWorkspace->deselectAll();
					fWorkspace->select(fDraggedIcon);
				}
			} else if (fStartZone == kStorage) {
				if (! e->modifiers().testFlag(Qt::ControlModifier)) {
					// use ctrl to duplicate an icon on the workspace
					fSrcIcon = fDraggedIcon;
					fDraggedIcon = fWorkspace->dupIcon(fDraggedIcon);
					fWorkspace->deselectAll();
					fWorkspace->select(fDraggedIcon);
				}
			} else {
				fSrcIcon = fDraggedIcon;
				fDraggedIcon = fWorkspace->dupIcon(fDraggedIcon);
				fWorkspace->deselectAll();
				fWorkspace->select(fDraggedIcon);
				//fWorkspace->select(fDraggedIcon);
			}
		}
	}
}

void Dragger::drop(QMouseEvent*)
{
	if (fDragging) {
		if (fTargetIcon) {	
			fWorkspace->paletteCompute(fTargetIcon, fDraggedIcon);
			fWorkspace->remIcon(fTargetIcon);
			fWorkspace->remIcon(fDraggedIcon);
		} else {
			int dropzone = fDraggedIcon->zone();
			switch (dropzone) {
				case kHistory 	: cancel(); break;
				case kPrimitives: cancel(); break;
				case kStorage 	: 
					if (fStartZone == kStorage) { 
						assert(fSrcIcon); 
						fWorkspace->remIcon(fSrcIcon); 
					} 
					break;
			}
		}
	}
	fDragging = false;
	fTargetIcon = NULL;
	fDraggedIcon = NULL;
}

void Dragger::cancel()
{
	if (fSrcIcon) {
		fWorkspace->remIcon(fDraggedIcon);
	} else {
		fDraggedIcon->move(fDraggedWidgetOrigin);
	}
}
