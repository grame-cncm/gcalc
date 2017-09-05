#ifndef __TERMS__
#define __TERMS__



#include <QPainter>
#include <QString>
#include "cubes.h"

long UsedElements();
void InitExp();


Tree* HorzPrimExp();
Tree* VertPrimExp();
Tree* ProfPrimExp();


Tree* LeftOpExp(int i);
Tree* RightOpExp(int i);

Tree* HorzExp(Tree* e1, Tree* e2);
Tree* VertExp(Tree* e1, Tree* e2);
Tree* ProfExp(Tree* e1, Tree* e2);
Tree* ApplExp(Tree* e1, Tree* e2);
Tree* AbstrExp(Tree* part, Tree* body);

void Represent(QPainter& painter, Tree* e, int size, int evalFlag);
void WritePOV(Tree* e, const QString& fname);


#endif

