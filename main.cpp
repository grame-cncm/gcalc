//---------------------------------------------------------------------
//---------------------------------------------------------------------
// This example shows how to create a window with a rayures background
// with Qt4 with another widget inside
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#include <QApplication>
#include <QWidget>

#include "WorkspaceWidget.h"
#include "IconWidget.h"
#include "PaletteWidget.h"
#include "constructions.h"
#include "cubes.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	WorkspaceWidget* workspace = new WorkspaceWidget();

	workspace->show();
	int r = app.exec();

	delete workspace;
	return r;
}
