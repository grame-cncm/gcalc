# gcrender: offscreen tool built on the gcalc sources (see README.md)

TEMPLATE = app
TARGET = gcrender
CONFIG -= app_bundle
CONFIG += console
QT += widgets
ROOT = $$PWD/..
INCLUDEPATH += $$ROOT $$ROOT/widgets $$ROOT/tlib $$ROOT/terms $$ROOT/cubes

# Input
HEADERS += $$ROOT/cubes/CubeUtils.h \
           $$ROOT/terms/constructions.h \
           $$ROOT/terms/cubes.h \
           $$ROOT/terms/DebugTrace.h \
           $$ROOT/terms/EvalRules.h \
           $$ROOT/terms/evaluate.h \
           $$ROOT/terms/POVUtils.h \
           $$ROOT/terms/TFile.h \
           $$ROOT/tlib/application.h \
           $$ROOT/tlib/debruijn.h \
           $$ROOT/tlib/env.h \
           $$ROOT/tlib/node.h \
           $$ROOT/tlib/print.h \
           $$ROOT/tlib/symbol.h \
           $$ROOT/tlib/tlib.h \
           $$ROOT/tlib/tree.h \
           $$ROOT/widgets/Dragger.h \
           $$ROOT/widgets/HistoryWidget.h \
           $$ROOT/widgets/IconWidget.h \
           $$ROOT/widgets/PaletteWidget.h \
           $$ROOT/widgets/PixWidget.h \
           $$ROOT/widgets/PrimitivesWidget.h \
           $$ROOT/widgets/StoreWidget.h \
           $$ROOT/widgets/WorkspaceWidget.h \
           $$ROOT/images/rayures3.xpm \
           $$ROOT/images/history.xpm \
           $$ROOT/images/palette.xpm \
           $$ROOT/images/rayures4.xpm \
           $$ROOT/images/rayures2.xpm \
           $$ROOT/images/storage.xpm \
           $$ROOT/images/rayures1.xpm
SOURCES += $$PWD/render.cpp \
           $$ROOT/cubes/CubeUtils.cpp \
           $$ROOT/terms/constructions.cpp \
           $$ROOT/terms/cubes.cpp \
           $$ROOT/terms/DebugTrace.cpp \
           $$ROOT/terms/EvalRules.cpp \
           $$ROOT/terms/evaluate.cpp \
           $$ROOT/terms/POVUtils.cpp \
           $$ROOT/tlib/application.cpp \
           $$ROOT/tlib/debruijn.cpp \
           $$ROOT/tlib/env.cpp \
           $$ROOT/tlib/node.cpp \
           $$ROOT/tlib/print.cpp \
           $$ROOT/tlib/symbol.cpp \
           $$ROOT/tlib/tree.cpp \
           $$ROOT/widgets/Dragger.cpp \
           $$ROOT/widgets/HistoryWidget.cpp \
           $$ROOT/widgets/IconWidget.cpp \
           $$ROOT/widgets/PaletteWidget.cpp \
           $$ROOT/widgets/PixWidget.cpp \
           $$ROOT/widgets/PrimitivesWidget.cpp \
           $$ROOT/widgets/StoreWidget.cpp \
           $$ROOT/widgets/WorkspaceWidget.cpp
