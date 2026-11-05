// Drives gcalc's own WorkspaceWidget through its palette operations (the same
// calls a drag and drop makes) to build the diagonally divided cube of the
// ICMC 94 paper, then grabs the window into PNG files.

#include <QApplication>
#include <QPixmap>
#include <cstdio>

#include "WorkspaceWidget.h"
#include "IconWidget.h"
#include "PaletteWidget.h"
#include "tlib.h"
#include "cubes.h"
#include "constructions.h"

enum { OP_ABSTR = 0, OP_LEFT = 1, OP_FRONT = 2, OP_TOP = 3, OP_FUN = 4, OP_BOT = 5, OP_BACK = 6, OP_RIGHT = 7, OP_ARG = 8 };

static WorkspaceWidget* W;

static void settle() { for (int i = 0; i < 5; i++) QApplication::processEvents(); }

static IconWidget* put(Tree* t, int x, int y)
{
	IconWidget* i = new IconWidget(t);
	W->addIcon(x, y, i);
	return i;
}

static PaletteWidget* palette()
{
	foreach (QWidget* w, W->findChildren<QWidget*>())
		if (PaletteWidget* p = dynamic_cast<PaletteWidget*>(w)) return p;
	return 0;
}

// show the palette on target, highlight operation op, the dragged icon over that zone
static void hover(IconWidget* target, IconWidget* drag, int op)
{
	W->paletteShow(target);
	QRect g = palette()->geometry();
	int h = op / 3, v = op % 3;
	QPoint p(g.left() + (2 * h + 1) * g.width() / 6, g.top() + (2 * v + 1) * g.height() / 6);
	W->paletteHighlightZone(p);
	drag->move(p - QPoint(drag->width() / 2, drag->height() / 2));
	drag->raise();
	settle();
}

// what a drop does: compute, remove both operands; returns the new icon
static IconWidget* drop(IconWidget* target, IconWidget* drag, int op, QPoint at)
{
	hover(target, drag, op);
	W->paletteCompute(target, drag);
	W->remIcon(target);
	W->remIcon(drag);
	settle();
	IconWidget* r = 0;
	foreach (IconWidget* i, W->findChildren<IconWidget*>())
		if (i->zone() == kWorkspace && i->isVisible()) r = i;   // the one just added
	r->move(at);
	settle();
	return r;
}

static void grab(const char* fname)
{
	settle();
	W->grab().save(fname);
	printf("saved %s\n", fname);
}

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	const char* dir = argc > 1 ? argv[1] : ".";
	char f[1024];

	W = new WorkspaceWidget();
	W->show();
	settle();

	// 1. (red red)
	IconWidget* rr = drop(put(RedExp(), 200, 300), put(RedExp(), 300, 300), OP_FUN, QPoint(200, 200));
	// 2. green | (red red)
	IconWidget* top = drop(rr, put(GreenExp(), 300, 200), OP_LEFT, QPoint(200, 200));
	// 3. (red red) | white
	IconWidget* rr2 = drop(put(RedExp(), 200, 400), put(RedExp(), 300, 400), OP_FUN, QPoint(200, 400));
	IconWidget* bot = drop(rr2, put(WhiteExp(), 300, 400), OP_RIGHT, QPoint(200, 400));
	// 4. [green | (red red)] / [(red red) | white]
	IconWidget* body = drop(bot, top, OP_TOP, QPoint(250, 300));

	// screenshot: about to abstract red (red cube dragged on the top-left cell)
	IconWidget* red = put(RedExp(), 400, 300);
	hover(body, red, OP_ABSTR);
	snprintf(f, sizeof f, "%s/screenshot-abstraction.png", dir);
	grab(f);
	W->paletteHide();

	// 5. V = \red. body
	IconWidget* V = drop(body, red, OP_ABSTR, QPoint(250, 300));
	// 6. X = (V V)
	IconWidget* V2 = W->dupIcon(V);
	V2->move(450, 300);
	IconWidget* X = drop(V, V2, OP_FUN, QPoint(330, 230));
	for (int i = 0; i < 8; i++) X->zoomIn();

	// the objects A and B of the paper in the workspace, V and X kept in the storage
	Tree* A = VertExp(HorzExp(GreenExp(), RedExp()), HorzExp(RedExp(), WhiteExp()));
	Tree* B = VertExp(HorzExp(GreenExp(), A), HorzExp(A, WhiteExp()));
	IconWidget* iA = put(A, 180, 600); iA->zoomIn(); iA->zoomIn();
	IconWidget* iB = put(B, 330, 600); iB->zoomIn(); iB->zoomIn();
	const char* names[] = { "A", "B", "V", "X" };
	Tree* terms[] = { A, B, V->term(), X->term() };
	for (int i = 0; i < 4; i++) {
		setNameProperty(terms[i], names[i]);
		put(terms[i], 760 + (i % 2) * 110, 80 + (i / 2) * 110);
	}
	settle();
	W->deselectAll();
	snprintf(f, sizeof f, "%s/screenshot-diagonal.png", dir);
	grab(f);
	return 0;
}
