######################################################################
# Automatically generated by qmake (3.0) mer. avr. 9 08:50:32 2014
######################################################################

TEMPLATE = app
TARGET = gcalc
QT += widgets
INCLUDEPATH += . widgets tlib terms cubes

# Input
HEADERS += cubes/CubeUtils.h \
           terms/constructions.h \
           terms/cubes.h \
           terms/DebugTrace.h \
           terms/EvalRules.h \
           terms/evaluate.h \
           terms/POVUtils.h \
           terms/TFile.h \
           tlib/application.h \
           tlib/debruijn.h \
           tlib/env.h \
           tlib/node.h \
           tlib/print.h \
           tlib/symbol.h \
           tlib/tlib.h \
           tlib/tree.h \
           widgets/Dragger.h \
           widgets/HistoryWidget.h \
           widgets/IconWidget.h \
           widgets/PaletteWidget.h \
           widgets/PixWidget.h \
           widgets/PrimitivesWidget.h \
           widgets/StoreWidget.h \
           widgets/WorkspaceWidget.h \
           images/rayures3.xpm \
           images/history.xpm \
           images/palette.xpm \
           images/rayures4.xpm \
           images/rayures2.xpm \
           images/storage.xpm \
           images/rayures1.xpm
SOURCES += main.cpp \
           cubes/CubeUtils.cpp \
           terms/constructions.cpp \
           terms/cubes.cpp \
           terms/DebugTrace.cpp \
           terms/EvalRules.cpp \
           terms/evaluate.cpp \
           terms/POVUtils.cpp \
           tlib/application.cpp \
           tlib/debruijn.cpp \
           tlib/env.cpp \
           tlib/node.cpp \
           tlib/print.cpp \
           tlib/symbol.cpp \
           tlib/tree.cpp \
           widgets/Dragger.cpp \
           widgets/HistoryWidget.cpp \
           widgets/IconWidget.cpp \
           widgets/PaletteWidget.cpp \
           widgets/PixWidget.cpp \
           widgets/PrimitivesWidget.cpp \
           widgets/StoreWidget.cpp \
           widgets/WorkspaceWidget.cpp
