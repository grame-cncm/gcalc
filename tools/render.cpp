// Offscreen renderer for GCalc terms: builds a term with the language's own
// constructors (colored cubes, constructions, lambda-abstraction, application),
// evaluates it lazily and draws it with gcalc's Represent() into a PNG.

#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>
#include <cstring>
#include <cstdlib>

#include "tlib.h"
#include "cubes.h"
#include "constructions.h"
#include "EvalRules.h"
#include "debruijn.h"
#include "application.h"

// variable markers (any cube used as the "part" of an abstraction)
static Tree* X() { return BlueExp(); }   // the function itself (self-application)
static Tree* A() { return RedExp(); }    // first color
static Tree* B() { return GreenExp(); }  // second color

static Tree* H(Tree* l, Tree* r) { return construct(0, l, r); } // left | right
static Tree* V(Tree* t, Tree* b) { return construct(1, t, b); } // top / bottom
static Tree* P(Tree* f, Tree* b) { return construct(2, f, b); } // front - back

// 2x2x2 block: names are (x)(y)(z) with x: 0=left, y: 0=top, z: 0=front
static Tree* oct(Tree* o000, Tree* o001, Tree* o010, Tree* o011,
                 Tree* o100, Tree* o101, Tree* o110, Tree* o111)
{
	return H(V(P(o000, o001), P(o010, o011)),
	         V(P(o100, o101), P(o110, o111)));
}

static Tree* app(Tree* f, Tree* a) { return application(f, a); }

// F = \x.\a.\b. body ; object = F F c1 c2
static Tree* fractal(Tree* body, int c1, int c2)
{
	Tree* F = abstract(X(), abstract(A(), abstract(B(), body)));
	return app(app(app(F, F), coloredCube(c1)), coloredCube(c2));
}

// recursive call with swapped colors: x x b a
static Tree* R()  { return app(app(app(X(), X()), B()), A()); }
// recursive call keeping colors
static Tree* Rk() { return app(app(app(X(), X()), A()), B()); }

// triangle (staircase) in the front half: a above b on the left, c at bottom right
static Tree* tri3(Tree* a, Tree* b, Tree* c)
{
	Tree* O = InvisExp();
	return H(V(P(a, O), P(b, O)),
	         V(O,       P(c, O)));
}

// T = \a. triangle of a ;  Tc = \a.\b.\c. triangle with three parts
static Tree* T()  { return abstract(RedExp(), tri3(RedExp(), RedExp(), RedExp())); }
static Tree* Tc() { return abstract(RedExp(), abstract(BlueExp(), abstract(GreenExp(),
                           tri3(RedExp(), BlueExp(), GreenExp())))); }

static Tree* makeObject(const char* name, int c1, int c2)
{
	Tree* O = InvisExp();
	Tree* k1 = coloredCube(c1);
	Tree* k2 = coloredCube(c2);
	if (!strcmp(name, "tri1")) {          // T c1
		return app(T(), k1);
	} else if (!strcmp(name, "tri2")) {   // T (T c1)
		return app(T(), app(T(), k1));
	} else if (!strcmp(name, "tri3")) {   // T (T (T c1))
		return app(T(), app(T(), app(T(), k1)));
	} else if (!strcmp(name, "tri2c")) {  // Tc (T c1) (T c2) (T c1)
		return app(app(app(Tc(), app(T(), k1)), app(T(), k2)), app(T(), k1));
	} else if (!strcmp(name, "paperA")) {
		// ICMC 94, section 3.2.1: A = [green|red] / [red|white]
		return V(H(GreenExp(), RedExp()), H(RedExp(), WhiteExp()));
	} else if (!strcmp(name, "paperB")) {
		// B: the red parts of A replaced by A itself
		Tree* a = V(H(GreenExp(), RedExp()), H(RedExp(), WhiteExp()));
		return V(H(GreenExp(), a), H(a, WhiteExp()));
	} else if (!strcmp(name, "paperX")) {
		// V = \red. [green | (red red)] / [(red red) | white] ;  X = (V V)
		Tree* rr = app(RedExp(), RedExp());
		Tree* Vf = abstract(RedExp(), V(H(GreenExp(), rr), H(rr, WhiteExp())));
		return app(Vf, Vf);
	} else if (!strncmp(name, "sponge", 6)) {
		// ICMC 94, section 3.2.3: booleans as selectors, AND, three objects A, B, C
		// on complementary axes, X = \white.\invisible.((AND A (AND B C)) white invisible)
		Tree* blue = BlueExp(); Tree* green = GreenExp();
		Tree* Tb  = abstract(blue, abstract(green, blue));
		Tree* Fb  = abstract(blue, abstract(green, green));
		Tree* AND = abstract(blue, abstract(green, app(app(blue, green), blue)));
		// a 4x4 section T / T|F|F|T / T|F|F|T / T in the plane (d1,d2), constant along the third axis
		auto section = [&](int d1, int d2) {
			Tree* row = construct(d1, construct(d1, Tb, Fb), construct(d1, Fb, Tb));
			return construct(d2, construct(d2, Tb, row), construct(d2, row, Tb));
		};
		Tree* A = section(0, 1);
		Tree* B = section(2, 1);
		Tree* C = section(0, 2);
		Tree* W = WhiteExp(); Tree* I = InvisExp();
		Tree* Xf = abstract(W, abstract(I, app(app(app(app(AND, A), app(app(AND, B), C)), W), I)));
		if (!strcmp(name, "sponge0")) return app(app(app(app(AND, A), app(app(AND, B), C)), k1), I);
		if (!strcmp(name, "sponge1")) return app(app(Xf, k1), I);
		// SPONGE = (((X X F) X F) white invisible), with white replaced by k1
		return app(app(app(app(app(app(Xf, Xf), Fb), Xf), Fb), k1), I);
	} else if (!strcmp(name, "diag")) {
		// D = \x.\a.\b. (x x a b | b) / (a | x x a b) ; object = D D c1 c2
		// two full triangular prisms of colors c1 (bottom left) and c2 (top right)
		Tree* r = app(app(app(X(), X()), A()), B());
		Tree* D = abstract(X(), abstract(A(), abstract(B(), H(V(r, A()), V(B(), r)))));
		return app(app(app(D, D), k1), k2);
	} else if (!strcmp(name, "diag2")) {
		// the other diagonal: (a | x x a b) / (x x a b | b)
		Tree* r = app(app(app(X(), X()), A()), B());
		Tree* D = abstract(X(), abstract(A(), abstract(B(), H(V(A(), r), V(r, B())))));
		return app(app(app(D, D), k1), k2);
	} else if (!strcmp(name, "tri2rgb")) {  // Tc (T red) (T blue) (T green)
		return app(app(app(Tc(), app(T(), RedExp())), app(T(), BlueExp())), app(T(), GreenExp()));
	} else if (!strcmp(name, "nest")) {   // big triangle of c1 with a small triangle of c2 in its corner
		return app(app(app(Tc(), app(T(), k2)), k1), k1);
	} else if (!strcmp(name, "ziggurat")) {
		// bottom layer: three recursive octants around a solid one, top: one recursive
		return fractal(oct(O, R(),  R(), A(),
		                   O, O,    R(), R()), c1, c2);
	} else if (!strcmp(name, "sierpinski")) {
		// 3D checkerboard: 4 recursive octants, with a solid core in the 4 others' place
		return fractal(oct(R(), O, O, R(),
		                   O, R(), R(), B()), c1, c2);
	} else if (!strcmp(name, "tower")) {
		return fractal(oct(R(), O,  A(), R(),
		                   O, R(),  R(), B()), c1, c2);
	} else if (!strcmp(name, "corner")) {
		// a solid corner, three recursive arms
		return fractal(oct(O, Rk(), Rk(), A(),
		                   O, O,    O,    Rk()), c1, c2);
	} else if (!strcmp(name, "crystal")) {
		// the near corner removed, at every scale; same colors at all levels except a solid far core
		return fractal(oct(Rk(), Rk(), Rk(), B(),
		                   O,    Rk(), A(),  Rk()), c1, c2);
	} else if (!strcmp(name, "steps")) {
		return fractal(oct(R(), A(), R(), A(),
		                   O,   R(), R(), B()), c1, c2);
	} else {
		// "frame": the 7 outer octants of a cube recurse, the inner front-top-right is empty
		return fractal(oct(R(), R(), R(), A(),
		                   O,   R(), R(), R()), c1, c2);
	}
}

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	const char* name = argc > 1 ? argv[1] : "frame";
	const char* out  = argc > 2 ? argv[2] : "out.png";
	int size  = argc > 3 ? atoi(argv[3]) : 600;    // logical size: sets the depth
	int scale = argc > 4 ? atoi(argv[4]) : 3;      // output pixels per logical pixel
	int c1 = argc > 5 ? (int)strtoul(argv[5], 0, 16) : 0xE8A33DFF;
	int c2 = argc > 6 ? (int)strtoul(argv[6], 0, 16) : 0x2F8F9DFF;

	int W = size * scale, M = W / 8;
	QImage img(W + 2 * M, W + 2 * M, QImage::Format_ARGB32_Premultiplied);
	QPainter p(&img);
	p.setRenderHint(QPainter::Antialiasing, false);
	QRadialGradient bg(img.width() * 0.45, img.height() * 0.40, img.width() * 0.75);
	bg.setColorAt(0, QColor(0x3a, 0x3f, 0x4f));
	bg.setColorAt(1, QColor(0x0e, 0x10, 0x16));
	p.fillRect(img.rect(), bg);

	p.translate(M, M);
	p.scale(scale, scale);
	Represent(p, makeObject(name, c1, c2), size, true);
	p.end();
	img.save(out);
	return 0;
}
